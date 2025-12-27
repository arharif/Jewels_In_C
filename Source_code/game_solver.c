#include "game_solver.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* ============================================================================
 * Hash table implementation (open addressing, stores a 64-bit fingerprint)
 * ============================================================================
 * Note: This matches the prototypes in game_solver.h.
 * If your project already provides these functions elsewhere, REMOVE ONE copy
 * to avoid "multiple definition" linker errors.
 */

static ulong fnv1a64_ints(const int *a, int n)
{
    uint64_t h = 1469598103934665603ULL;
    for (int i = 0; i < n; i++) {
        uint32_t v = (uint32_t)a[i];
        h ^= (uint64_t)v;
        h *= 1099511628211ULL;
    }
    if (h == 0) h = 1; /* 0 used as empty sentinel */
    return (ulong)h;
}

static void ht_rehash(hash_table *ht, ulong newcap)
{
    ulong *old = ht->hash_table;
    ulong oldcap = ht->hash_capacity;

    ht->hash_table = (ulong *)calloc((size_t)newcap, sizeof(ulong));
    if (!ht->hash_table) exit(EXIT_FAILURE);

    ht->hash_capacity = newcap;
    ht->hash_size = 0;

    for (ulong i = 0; i < oldcap; i++) {
        ulong key = old[i];
        if (key == 0) continue;

        ulong pos = key % ht->hash_capacity;
        while (ht->hash_table[pos] != 0) {
            pos = (pos + 1) % ht->hash_capacity;
        }
        ht->hash_table[pos] = key;
        ht->hash_size++;
    }

    free(old);
}

hash_table *create_hash_table(int element_size)
{
    hash_table *ht = (hash_table *)calloc(1, sizeof(*ht));
    if (!ht) return NULL;

    ht->element_size = element_size;
    ht->hash_capacity = 1024;
    ht->hash_size = 0;
    ht->hash_table = (ulong *)calloc((size_t)ht->hash_capacity, sizeof(ulong));
    if (!ht->hash_table) {
        free(ht);
        return NULL;
    }
    return ht;
}

void delete_hash_table(hash_table *ht)
{
    if (!ht) return;
    free(ht->hash_table);
    free(ht);
}

/* Returns true if inserted (new), false if already present */
bool hash_table_insert(hash_table *ht, int *elements, ulong i)
{
    (void)i; /* id not needed when we store fingerprints */
    if (!ht || !elements) return false;

    /* grow if load factor > ~0.7 */
    if (ht->hash_size * 10 >= ht->hash_capacity * 7) {
        ht_rehash(ht, ht->hash_capacity * 2);
    }

    ulong key = fnv1a64_ints(elements, ht->element_size);

    ulong pos = key % ht->hash_capacity;
    while (1) {
        if (ht->hash_table[pos] == 0) {
            ht->hash_table[pos] = key;
            ht->hash_size++;
            return true;
        }
        if (ht->hash_table[pos] == key) {
            return false; /* already visited */
        }
        pos = (pos + 1) % ht->hash_capacity;
    }
}

/* ============================================================================
 * Utilities
 * ============================================================================
 */

static inline bool in_bounds(const board *b, int x, int y)
{
    return (b && x >= 0 && y >= 0 && x < b->largeur && y < b->hauteur);
}

static inline bool is_matchable(element e)
{
    return (e >= AMBER && e <= TOPAZ);
}

static int run_length_axis(const board *b, int x, int y, int dx, int dy)
{
    if (!in_bounds(b, x, y)) return 0;

    element e = b->plateau[y][x].object;
    if (!is_matchable(e)) return 0;

    int count = 1;

    int xf = x + dx, yf = y + dy;
    while (in_bounds(b, xf, yf) && b->plateau[yf][xf].object == e) {
        count++;
        xf += dx; yf += dy;
    }

    int xb = x - dx, yb = y - dy;
    while (in_bounds(b, xb, yb) && b->plateau[yb][xb].object == e) {
        count++;
        xb -= dx; yb -= dy;
    }

    return count;
}

static bool is_in_match(const board *b, int x, int y)
{
    if (!in_bounds(b, x, y)) return false;

    element e = b->plateau[y][x].object;
    if (!is_matchable(e)) return false;

    if (run_length_axis(b, x, y, 1, 0) >= 3) return true;
    if (run_length_axis(b, x, y, 0, 1) >= 3) return true;

    return false;
}

/* ============================================================================
 * Deterministic puzzle stabilization (no random refill)
 * ============================================================================
 */

static bool mark_kill_local(board *b)
{
    if (!b) return false;

    bool any = false;

    for (int y = 0; y < b->hauteur; y++)
        for (int x = 0; x < b->largeur; x++)
            b->plateau[y][x].kill = KILL_NOKILL;

    for (int y = 0; y < b->hauteur; y++) {
        for (int x = 0; x < b->largeur; x++) {
            if (is_in_match(b, x, y)) {
                b->plateau[y][x].kill = KILL_SHRINK;
                any = true;
            }
        }
    }
    return any;
}

static void apply_kill_local(board *b)
{
    if (!b) return;

    for (int y = 0; y < b->hauteur; y++) {
        for (int x = 0; x < b->largeur; x++) {
            if (b->plateau[y][x].kill != KILL_NOKILL) {
                b->plateau[y][x].object = EMPTY;
                b->plateau[y][x].kill   = KILL_NOKILL;
                b->plateau[y][x].fall   = 0;
                b->plateau[y][x].power  = GEM_BASIC;
            }
        }
    }
}

static bool compact_gravity_local(board *b)
{
    if (!b) return false;

    bool moved = false;

    switch (b->gravite)
    {
    case DOWN:
        for (int x = 0; x < b->largeur; x++) {
            int dst_y = b->hauteur - 1;
            for (int y = b->hauteur - 1; y >= 0; y--) {
                if (b->plateau[y][x].object != EMPTY) {
                    if (y != dst_y) {
                        b->plateau[dst_y][x] = b->plateau[y][x];
                        b->plateau[y][x].object = EMPTY;
                        b->plateau[y][x].power  = GEM_BASIC;
                        b->plateau[y][x].kill   = KILL_NOKILL;
                        moved = true;
                    }
                    dst_y--;
                }
            }
        }
        break;

    case UP:
        for (int x = 0; x < b->largeur; x++) {
            int dst_y = 0;
            for (int y = 0; y < b->hauteur; y++) {
                if (b->plateau[y][x].object != EMPTY) {
                    if (y != dst_y) {
                        b->plateau[dst_y][x] = b->plateau[y][x];
                        b->plateau[y][x].object = EMPTY;
                        b->plateau[y][x].power  = GEM_BASIC;
                        b->plateau[y][x].kill   = KILL_NOKILL;
                        moved = true;
                    }
                    dst_y++;
                }
            }
        }
        break;

    case RIGHT:
        for (int y = 0; y < b->hauteur; y++) {
            int dst_x = b->largeur - 1;
            for (int x = b->largeur - 1; x >= 0; x--) {
                if (b->plateau[y][x].object != EMPTY) {
                    if (x != dst_x) {
                        b->plateau[y][dst_x] = b->plateau[y][x];
                        b->plateau[y][x].object = EMPTY;
                        b->plateau[y][x].power  = GEM_BASIC;
                        b->plateau[y][x].kill   = KILL_NOKILL;
                        moved = true;
                    }
                    dst_x--;
                }
            }
        }
        break;

    case LEFT:
        for (int y = 0; y < b->hauteur; y++) {
            int dst_x = 0;
            for (int x = 0; x < b->largeur; x++) {
                if (b->plateau[y][x].object != EMPTY) {
                    if (x != dst_x) {
                        b->plateau[y][dst_x] = b->plateau[y][x];
                        b->plateau[y][x].object = EMPTY;
                        b->plateau[y][x].power  = GEM_BASIC;
                        b->plateau[y][x].kill   = KILL_NOKILL;
                        moved = true;
                    }
                    dst_x++;
                }
            }
        }
        break;

    default:
        break;
    }

    return moved;
}

static void stabilize_board_local(board *b)
{
    if (!b) return;

    compact_gravity_local(b);

    while (mark_kill_local(b)) {
        apply_kill_local(b);
        compact_gravity_local(b);
    }
}

/* ============================================================================
 * Goal + heuristic
 * ============================================================================
 */

static bool is_goal_state(const game *g)
{
    if (!g || !g->b) return false;
    const board *b = g->b;

    for (int y = 0; y < b->hauteur; y++)
        for (int x = 0; x < b->largeur; x++)
            if (b->plateau[y][x].object != EMPTY)
                return false;

    return true;
}

static int heuristic_nonempty(const game *g)
{
    if (!g || !g->b) return 0;
    const board *b = g->b;

    int h = 0;
    for (int y = 0; y < b->hauteur; y++)
        for (int x = 0; x < b->largeur; x++)
            if (b->plateau[y][x].object != EMPTY) h++;

    return h;
}

/* ============================================================================
 * Hash config
 * ============================================================================
 */

static int config_element_size(const game *g)
{
    if (!g || !g->b) return 0;
    return 1 + g->b->largeur * g->b->hauteur;
}

static void fill_config_elements(const game *g, int *out)
{
    const board *b = g->b;

    out[0] = (int)b->gravite;

    int idx = 1;
    for (int y = 0; y < b->hauteur; y++) {
        for (int x = 0; x < b->largeur; x++) {
            int obj = (int)b->plateau[y][x].object;
            int pow = (int)b->plateau[y][x].power;
            out[idx++] = (obj << 8) | (pow & 0xFF);
        }
    }
}

/* ============================================================================
 * Moves
 * ============================================================================
 */

static solver_move *make_move(solver_move_type type, int index, direction dir)
{
    solver_move *m = (solver_move *)calloc(1, sizeof(*m));
    if (!m) return NULL;
    m->type = type;
    m->index = index;
    m->dir = dir;
    m->next = NULL;
    return m;
}

static solver_move *reverse_moves(solver_move *head)
{
    solver_move *prev = NULL;
    while (head) {
        solver_move *n = head->next;
        head->next = prev;
        prev = head;
        head = n;
    }
    return prev;
}

/* ============================================================================
 * Node
 * ============================================================================
 */

typedef struct node {
    game *state;
    struct node *parent;
    solver_move *move_from_parent;
    int g_cost;
    int h_cost;
    int f_cost;
    ulong id;
} node;

/* ============================================================================
 * BFS queue / DFS stack / A* heap
 * ============================================================================
 */

typedef struct {
    node **data;
    int cap, head, tail, size;
} node_queue;

static void q_init(node_queue *q){ q->data=NULL; q->cap=q->head=q->tail=q->size=0; }
static void q_free(node_queue *q){ free(q->data); q->data=NULL; q->cap=q->head=q->tail=q->size=0; }

static void q_push(node_queue *q, node *n)
{
    if (q->size + 1 > q->cap) {
        int newcap = (q->cap==0)?128:q->cap*2;
        node **nd = (node **)realloc(q->data, (size_t)newcap*sizeof(node *));
        if (!nd) exit(EXIT_FAILURE);
        q->data = nd;
        if (q->cap > 0 && q->head > q->tail) {
            int right = q->cap - q->head;
            memmove(&q->data[newcap - right], &q->data[q->head], (size_t)right*sizeof(node *));
            q->head = newcap - right;
        }
        q->cap = newcap;
    }
    q->data[q->tail] = n;
    q->tail = (q->tail + 1) % q->cap;
    q->size++;
}

static node *q_pop(node_queue *q)
{
    if (q->size==0) return NULL;
    node *n = q->data[q->head];
    q->head = (q->head + 1) % q->cap;
    q->size--;
    return n;
}

typedef struct {
    node **data;
    int cap, size;
} node_stack;

static void st_init(node_stack *s){ s->data=NULL; s->cap=s->size=0; }
static void st_free(node_stack *s){ free(s->data); s->data=NULL; s->cap=s->size=0; }

static void st_push(node_stack *s, node *n)
{
    if (s->size+1 > s->cap) {
        int newcap = (s->cap==0)?128:s->cap*2;
        node **nd = (node **)realloc(s->data, (size_t)newcap*sizeof(node *));
        if (!nd) exit(EXIT_FAILURE);
        s->data = nd;
        s->cap = newcap;
    }
    s->data[s->size++] = n;
}

static node *st_pop(node_stack *s)
{
    if (s->size==0) return NULL;
    return s->data[--s->size];
}

typedef struct {
    node **data;
    int cap, size;
} node_heap;

static bool heap_less(const node *a, const node *b)
{
    if (a->f_cost != b->f_cost) return a->f_cost < b->f_cost;
    return a->h_cost < b->h_cost;
}

static void hp_init(node_heap *h){ h->data=NULL; h->cap=h->size=0; }
static void hp_free(node_heap *h){ free(h->data); h->data=NULL; h->cap=h->size=0; }

static void hp_push(node_heap *h, node *n)
{
    if (h->size+1 > h->cap) {
        int newcap = (h->cap==0)?128:h->cap*2;
        node **nd = (node **)realloc(h->data, (size_t)newcap*sizeof(node *));
        if (!nd) exit(EXIT_FAILURE);
        h->data = nd;
        h->cap = newcap;
    }
    int i = h->size++;
    h->data[i] = n;

    while (i > 0) {
        int p = (i - 1) / 2;
        if (!heap_less(h->data[i], h->data[p])) break;
        node *tmp = h->data[i]; h->data[i] = h->data[p]; h->data[p] = tmp;
        i = p;
    }
}

static node *hp_pop(node_heap *h)
{
    if (h->size==0) return NULL;
    node *root = h->data[0];
    h->data[0] = h->data[--h->size];

    int i = 0;
    while (1) {
        int l = 2*i+1, r = 2*i+2, m=i;
        if (l < h->size && heap_less(h->data[l], h->data[m])) m = l;
        if (r < h->size && heap_less(h->data[r], h->data[m])) m = r;
        if (m==i) break;
        node *tmp = h->data[i]; h->data[i] = h->data[m]; h->data[m] = tmp;
        i = m;
    }
    return root;
}

/* ============================================================================
 * Apply solver move (public API)
 * ============================================================================
 */

bool game_implement_solver_move(game *g, solver_move *move)
{
    if (!g || !g->b || !move) return false;
    board *b = g->b;

    int w = b->largeur;

    switch (move->type)
    {
    case SOLVER_SWAP: {
        int x = move->index % w;
        int y = move->index / w;

        int nx = x, ny = y;
        if (move->dir == UP) ny--;
        else if (move->dir == DOWN) ny++;
        else if (move->dir == LEFT) nx--;
        else if (move->dir == RIGHT) nx++;

        if (!in_bounds(b, x, y) || !in_bounds(b, nx, ny)) return false;

        cell tmp = b->plateau[y][x];
        b->plateau[y][x] = b->plateau[ny][nx];
        b->plateau[ny][nx] = tmp;

        if (!is_in_match(b, x, y) && !is_in_match(b, nx, ny)) {
            tmp = b->plateau[y][x];
            b->plateau[y][x] = b->plateau[ny][nx];
            b->plateau[ny][nx] = tmp;
            return false;
        }

        stabilize_board_local(b);
        return true;
    }

    case SOLVER_GRAVITY:
        if (move->dir < 0 || move->dir >= SIZE_DIRECTION) return false;
        b->gravite = move->dir;
        stabilize_board_local(b);
        return true;

    case SOLVER_CYCLE:
        b->gravite = (direction)((b->gravite + 1) % 4);
        stabilize_board_local(b);
        return true;

    case SOLVER_STORAGE:
        /* Storage rules are project-specific. */
        return false;

    default:
        return false;
    }
}

/* ============================================================================
 * Reconstruct solution
 * ============================================================================
 */

static solver_move *reconstruct_solution(node *goal, int *nb_moves)
{
    solver_move *path = NULL;
    int count = 0;

    node *cur = goal;
    while (cur && cur->move_from_parent) {
        solver_move *copy = make_move(cur->move_from_parent->type,
                                      cur->move_from_parent->index,
                                      cur->move_from_parent->dir);
        if (!copy) exit(EXIT_FAILURE);
        copy->next = path;
        path = copy;
        count++;
        cur = cur->parent;
    }

    path = reverse_moves(path);
    if (nb_moves) *nb_moves = count;
    return path;
}

/* ============================================================================
 * Successor expansion helper (safe naming: no TRY_MOVE macro conflicts)
 * ============================================================================
 */

typedef struct {
    solver method;
    hash_table *visited;
    int *scratch;
    ulong *next_id;
    int *nb_configs;
    node *parent;
    void *frontier; /* node_queue* or node_stack* or node_heap* */
} expand_ctx;

static void frontier_push(expand_ctx *ctx, node *n)
{
    if (ctx->method == SOLVER_BFS) q_push((node_queue *)ctx->frontier, n);
    else if (ctx->method == SOLVER_DFS) st_push((node_stack *)ctx->frontier, n);
    else hp_push((node_heap *)ctx->frontier, n);
}

static void expand_try_one_move(expand_ctx *ctx, game *cur, solver_move *mv)
{
    if (!ctx || !cur || !mv) return;

    game *ng = game_clone(cur);
    if (!ng) { free(mv); return; }

    if (!game_implement_solver_move(ng, mv)) {
        game_release(ng);
        free(mv);
        return;
    }

    fill_config_elements(ng, ctx->scratch);

    ulong id = (*ctx->next_id)++;
    if (!hash_table_insert(ctx->visited, ctx->scratch, id)) {
        game_release(ng);
        free(mv);
        return;
    }

    node *nn = (node *)calloc(1, sizeof(*nn));
    if (!nn) exit(EXIT_FAILURE);

    nn->state = ng;
    nn->parent = ctx->parent;
    nn->move_from_parent = mv;
    nn->g_cost = ctx->parent->g_cost + 1;
    nn->h_cost = heuristic_nonempty(ng);
    nn->f_cost = nn->g_cost + nn->h_cost;
    nn->id = id;

    (*ctx->nb_configs)++;
    frontier_push(ctx, nn);
}

static void push_successors(game *cur, node *cur_node, solver method,
                            hash_table *visited, int *scratch, ulong *next_id,
                            int *nb_configs, void *frontier)
{
    board *b = cur->b;
    int w = b->largeur;
    int h = b->hauteur;

    expand_ctx ctx;
    ctx.method = method;
    ctx.visited = visited;
    ctx.scratch = scratch;
    ctx.next_id = next_id;
    ctx.nb_configs = nb_configs;
    ctx.parent = cur_node;
    ctx.frontier = frontier;

    /* SWAPS (only RIGHT and DOWN to reduce duplicates) */
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int idx = y * w + x;
            if (x + 1 < w) expand_try_one_move(&ctx, cur, make_move(SOLVER_SWAP, idx, RIGHT));
            if (y + 1 < h) expand_try_one_move(&ctx, cur, make_move(SOLVER_SWAP, idx, DOWN));
        }
    }

    /* Set gravity */
    expand_try_one_move(&ctx, cur, make_move(SOLVER_GRAVITY, 0, UP));
    expand_try_one_move(&ctx, cur, make_move(SOLVER_GRAVITY, 0, RIGHT));
    expand_try_one_move(&ctx, cur, make_move(SOLVER_GRAVITY, 0, DOWN));
    expand_try_one_move(&ctx, cur, make_move(SOLVER_GRAVITY, 0, LEFT));

    /* Cycle gravity */
    expand_try_one_move(&ctx, cur, make_move(SOLVER_CYCLE, 0, UP));

    /* Storage disabled until your exact storage rules are implemented */
}

/* ============================================================================
 * Main solve function
 * ============================================================================
 */

solver_move *solve_puzzle(game *g, solver method, int bound,
                          solver_result_type *result, int *nb_moves,
                          int *nb_configs)
{
    if (result) *result = SOLVER_UNSOLVABLE;
    if (nb_moves) *nb_moves = 0;
    if (nb_configs) *nb_configs = 0;

    if (!g || !g->b) return NULL;

    game *start = game_clone(g);
    if (!start) return NULL;

    stabilize_board_local(start->b);

    if (is_goal_state(start)) {
        game_release(start);
        if (result) *result = SOLVER_SOLVED;
        return NULL;
    }

    int esz = config_element_size(start);
    int *scratch = (int *)malloc((size_t)esz * sizeof(int));
    if (!scratch) exit(EXIT_FAILURE);

    hash_table *visited = create_hash_table(esz);
    if (!visited) exit(EXIT_FAILURE);

    ulong next_id = 1;

    fill_config_elements(start, scratch);
    (void)hash_table_insert(visited, scratch, next_id++);

    node *start_node = (node *)calloc(1, sizeof(*start_node));
    if (!start_node) exit(EXIT_FAILURE);

    start_node->state = start;
    start_node->parent = NULL;
    start_node->move_from_parent = NULL;
    start_node->g_cost = 0;
    start_node->h_cost = heuristic_nonempty(start);
    start_node->f_cost = start_node->h_cost;
    start_node->id = 0;

    int configs = 1;
    int explored = 0;

    solver_move *solution = NULL;

    if (method == SOLVER_BFS) {
        node_queue q; q_init(&q);
        q_push(&q, start_node);

        while (q.size > 0) {
            if (bound > 0 && explored >= bound) {
                if (result) *result = SOLVER_TIMEOUT;
                break;
            }

            node *cur = q_pop(&q);
            explored++;

            if (is_goal_state(cur->state)) {
                if (result) *result = SOLVER_SOLVED;
                solution = reconstruct_solution(cur, nb_moves);
                break;
            }

            push_successors(cur->state, cur, method, visited, scratch, &next_id,
                            &configs, &q);
        }
        q_free(&q);
    }
    else if (method == SOLVER_DFS) {
        node_stack st; st_init(&st);
        st_push(&st, start_node);

        while (st.size > 0) {
            if (bound > 0 && explored >= bound) {
                if (result) *result = SOLVER_TIMEOUT;
                break;
            }

            node *cur = st_pop(&st);
            explored++;

            if (is_goal_state(cur->state)) {
                if (result) *result = SOLVER_SOLVED;
                solution = reconstruct_solution(cur, nb_moves);
                break;
            }

            push_successors(cur->state, cur, method, visited, scratch, &next_id,
                            &configs, &st);
        }
        st_free(&st);
    }
    else { /* SOLVER_ASTAR */
        node_heap hp; hp_init(&hp);
        hp_push(&hp, start_node);

        while (hp.size > 0) {
            if (bound > 0 && explored >= bound) {
                if (result) *result = SOLVER_TIMEOUT;
                break;
            }

            node *cur = hp_pop(&hp);
            explored++;

            if (is_goal_state(cur->state)) {
                if (result) *result = SOLVER_SOLVED;
                solution = reconstruct_solution(cur, nb_moves);
                break;
            }

            push_successors(cur->state, cur, method, visited, scratch, &next_id,
                            &configs, &hp);
        }
        hp_free(&hp);
    }

    if (nb_configs) *nb_configs = configs;

    delete_hash_table(visited);
    free(scratch);

    return solution;
}