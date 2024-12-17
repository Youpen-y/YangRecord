// code from memcached
#ifndef BIPBUFFER_H
#define BIPBUFFER_H

typedef struct {
    unsigned long int size;

    /* region A */
    unsigned int a_start, a_end;

    /* region B */
    unsigned int b_end;

    /* is B inuse? */
    int b_inuse;

    unsigned char data[];
} bipbuf_t;


/**
 * Create a new bip buffer
 * 
 * malloc() space
 * 
 * @param size, the size of the buffer
 */
bipbuf_t *bipbuf_new(const unsigned int size);

/**
 * Initialise a bip buffer. Use memory provided by user.
 *
 * No malloc()s are performed.
 *
 * @param size, the size of the array
 */
void bipbuf_init(bipbuf_t* me, const unsigned int size);

/**
 * Free the bip buffer
 */
void bipbuf_free(bipbuf_t* me);

/**
 * Request buffer space(reserve)
 * @param size, the size of requested space
 * @return 0 if no enough space, start address of chosen region on success 
 */
unsigned char *bipbuf_request(bipbuf_t* me, const int size);

/**
 * Extend region (move index)
 *
 * @param size, size of data
 * @return size on success, 0 if failed
 */
int bipbuf_push(bipbuf_t* me, const int size);

/**
 * @param data, the data to be offered to the buffer
 * @param size, the size of the data to be offered
 * @return number of bytes offered
 */
int bipbuf_offer(bipbuf_t* me, const unsigned char *data, const int size);

/**
 * Look at data. Don't move cursor
 *
 * @param len, the length of the data to be peeked
 * @return data on success, NULL if we can't peek at this much data
 */
unsigned char *bipbuf_peek(const bipbuf_t* me, const unsigned int len);

/**
 * Look at data. Don't move cursor
 *
 * @param len, The length of the data returned
 * @return data on success, NULL if nothing available
 */
unsigned char *bipbuf_peek_all(const bipbuf_t* me, unsigned int *len);

/**
 * Get pointer to data to read. Move the cursor on.
 *
 * @param len, The length of the data to be polled
 * @return pointer to data, NULL if we can't poll this much data
 */
unsigned char *bipbuf_poll(bipbuf_t* me, const unsigned int size);

/**
 * @return the size of the bipbuffer
 */
int bipbuf_size(const bipbuf_t* me);

/**
 * @return 1 if buffer is empty; 0 otherwise
 */
int bipbuf_is_empty(const bipbuf_t* me);

/**
 * @return how much space we have assigned
 */
int bipbuf_used(const bipbuf_t* cb);

/**
 * @return bytes of unused space
 */
int bipbuf_unused(const bipbuf_t* me);

#endif /*BIPBUFFER_H */












