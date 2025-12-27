/**
 * @file gui_textbuf.h
 * @brief
 * Text inputs. DO NOT MODIFY.
 *
 * @attention
 * This part is already implemented. It should not be modified.
 */

#ifndef _TEXTBUF_H_
#define _TEXTBUF_H_

#include <stdlib.h>
#include <string.h>
#include "alloc.h"

 /*  _____         _     _                   _    */
 /* |_   _|____  _| |_  (_)_ __  _ __  _   _| |_  */
 /*   | |/ _ \ \/ / __| | | '_ \| '_ \| | | | __| */
 /*   | |  __/>  <| |_  | | | | | |_) | |_| | |_  */
 /*   |_|\___/_/\_\\__| |_|_| |_| .__/ \__,_|\__| */
 /*                             |_|               */

/**
 * @brief The text buffer structure.
*/
typedef struct {
  char* content; //!< The content of the buffer.
  int capacity; //!< The capacity of the buffer.
  int length; //!< The length of the buffer.
} text_buffer;


/**
 * @brief Creation of a buffer.
*/
text_buffer* create_text_buffer(void);

/**
 * @brief Free the memory allocated for a buffer.
*/
void free_text_buffer(text_buffer* //!< The buffer.
);

/**
 * @brief Expands the capacity of a buffer.
*/
void expand_text_buffer(text_buffer* //!< The buffer.
  , int //!< The increase in capacity.
);


/**
 * @brief Appends a string to a buffer.
*/
void append_to_text_buffer(text_buffer* //!< The buffer.
  , char* //!< The string to append.
);

/**
 * @brief Removes the last character of a buffer.
*/
void remove_char(text_buffer* //!< The buffer.
);

#endif // _TEXTBUF_H_
