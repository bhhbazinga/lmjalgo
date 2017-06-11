/* Copyright (c) 2013, Ben Noordhuis <info@bnoordhuis.nl>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef QUEUE_H_
#define QUEUE_H_

typedef void *QUEUE[2];

/* Private macros. */
#define QUEUE_NEXT(q)       (*(QUEUE **) &((*(q))[0]))
#define QUEUE_PREV(q)       (*(QUEUE **) &((*(q))[1]))
#define QUEUE_PREV_NEXT(q)  (QUEUE_NEXT(QUEUE_PREV(q)))
#define QUEUE_NEXT_PREV(q)  (QUEUE_PREV(QUEUE_NEXT(q)))

/* Public macros. */
#define QUEUE_DATA(ptr, type, field)                                          \
	((type *) ((char *) (ptr) - ((char *) &((type *) 0)->field)))

#define QUEUE_FOREACH(q, h)                                                   \
	for ((q) = QUEUE_NEXT(h); (q) != (h); (q) = QUEUE_NEXT(q))

#define QUEUE_FOREACH_R(q, h)                                                 \
	for ((q) = QUEUE_PREV(h); (q) != (h); (q) = QUEUE_PREV(q))

#define QUEUE_EMPTY(q)                                                        \
	((const QUEUE *) (q) == (const QUEUE *) QUEUE_NEXT(q))

#define QUEUE_HEAD(q)                                                         \
	(QUEUE_NEXT(q))

#define QUEUE_TAIL(q)                                                         \
	(QUEUE_PREV(q))

#define QUEUE_INIT(q)                                                         \
	do {                                                                        \
		QUEUE_NEXT(q) = (q);                                                      \
		QUEUE_PREV(q) = (q);                                                      \
	}                                                                           \
while (0)

#define QUEUE_APPEND(h, n)							\
	do {										\
		QUEUE_NEXT(n) = QUEUE_NEXT(h);					\
		QUEUE_NEXT_PREV(h) = n;						\
		QUEUE_NEXT(h) = n;							\
		QUEUE_PREV(n) = h;							\
	} while (0)

#define QUEUE_ADD(h, n)                                                       \
	do {                                                                        \
		QUEUE_PREV_NEXT(h) = QUEUE_NEXT(n);                                       \
		QUEUE_NEXT_PREV(n) = QUEUE_PREV(h);                                       \
		QUEUE_PREV(h) = QUEUE_PREV(n);                                            \
		QUEUE_PREV_NEXT(h) = (h);                                                 \
	}                                                                           \
while (0)

#define QUEUE_SPLIT(h, q, n)                                                  \
	do {                                                                        \
		QUEUE_PREV(n) = QUEUE_PREV(h);                                            \
		QUEUE_PREV_NEXT(n) = (n);                                                 \
		QUEUE_NEXT(n) = (q);                                                      \
		QUEUE_PREV(h) = QUEUE_PREV(q);                                            \
		QUEUE_PREV_NEXT(h) = (h);                                                 \
		QUEUE_PREV(q) = (n);                                                      \
	}                                                                           \
while (0)

#define QUEUE_INSERT_HEAD(h, q)                                               \
	do {                                                                        \
		QUEUE_NEXT(q) = QUEUE_NEXT(h);                                            \
		QUEUE_PREV(q) = (h);                                                      \
		QUEUE_NEXT_PREV(q) = (q);                                                 \
		QUEUE_NEXT(h) = (q);                                                      \
	}                                                                           \
while (0)

#define QUEUE_INSERT_TAIL(h, q)                                               \
	do {                                                                        \
		QUEUE_NEXT(q) = (h);                                                      \
		QUEUE_PREV(q) = QUEUE_PREV(h);                                            \
		QUEUE_PREV_NEXT(q) = (q);                                                 \
		QUEUE_PREV(h) = (q);                                                      \
	}                                                                           \
while (0)

#define QUEUE_SWAP(q, p)						\
	do {									\
		QUEUE *t1, *t2;							\
		if (QUEUE_NEXT(q) != p && QUEUE_PREV(q) != p) {			\
			t1 = QUEUE_PREV(q);					\
			t2 = QUEUE_PREV(p);					\
			QUEUE_REMOVE(q);					\
			QUEUE_REMOVE(p);					\
			QUEUE_APPEND(t1, p);					\
			QUEUE_APPEND(t2, q);					\
		} else {							\
			if (QUEUE_NEXT(p) == q) {				\
				t1 = p;						\
				p = q;						\
				q = t1;						\
			}							\
			QUEUE_PREV_NEXT(q) = p;					\
			QUEUE_PREV(p) = QUEUE_PREV(q);				\
			QUEUE_NEXT_PREV(p) = q;					\
			QUEUE_NEXT(q) = QUEUE_NEXT(p);				\
			QUEUE_NEXT(p) = q;					\
			QUEUE_PREV(q) = p;					\
		}								\
	}while (0)

#define QUEUE_REMOVE(q)                                                       \
	do {                                                                        \
		QUEUE_PREV_NEXT(q) = QUEUE_NEXT(q);                                       \
		QUEUE_NEXT_PREV(q) = QUEUE_PREV(q);                                       \
	}                                                                           \
while (0)

#define QUEUE_DO_NOTHING(...) do {} while(0)
#define QDATA_FREE(p, u) free(p)

#define QUEUE_CLEAR(list, type, field, free_func, udata)		\
	do {									\
		QUEUE *q = QUEUE_HEAD(list);					\
		while (q != list) {						\
			type *p = QUEUE_DATA(q, type, field);			\
			q = QUEUE_NEXT(q);					\
			QUEUE_REMOVE(&(p->field));				\
			free_func(p, udata);					\
		}								\
	} while (0)

inline static size_t QUEUE_LEN(QUEUE *q)
{
	QUEUE *h = NULL;
	size_t len = 0;
	QUEUE_FOREACH(h, q) {
		len++;
	}
	return len;
}

#endif /* QUEUE_H_ */
