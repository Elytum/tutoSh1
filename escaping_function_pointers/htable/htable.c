#include <htable.h>

#include <string.h>
#include <limits.h>

/* Create a new hashtable. */
hashtable_t *ht_create( int size ) {

	hashtable_t *hashtable = NULL;
	int i;

	if( size < 1 ) return NULL;

	/* Allocate the table itself. */
	if( ( hashtable = malloc( sizeof( hashtable_t ) ) ) == NULL ) {
		return NULL;
	}

	/* Allocate pointers to the head nodes. */
	if( ( hashtable->table = malloc( sizeof( entry_t * ) * size ) ) == NULL ) {
		return NULL;
	}
	for( i = 0; i < size; i++ ) {
		hashtable->table[i] = NULL;
	}

	hashtable->size = size;

	return hashtable;	
}

/* Hash a string for a particular hash table. */
int ht_hash( hashtable_t *hashtable, char *key ) {

	unsigned long int hashval;
	unsigned long int i;

	hashval = 0;
	i = 0;
	/* Convert our string to an integer */
	while( hashval < ULONG_MAX && i < strlen( key ) ) {
		hashval = hashval << 8;
		hashval += key[ i ];
		i++;
	}

	return hashval % hashtable->size;
}

/* Create a key-value pair. */
entry_t *ht_newpair( char *key, void *value ) {
	entry_t *newpair;

	if( ( newpair = malloc( sizeof( entry_t ) ) ) == NULL ) {
		return NULL;
	}

	if( ( newpair->key = key ) == NULL ) {
		return NULL;
	}

	if( ( newpair->value = value ) == NULL ) {
		return NULL;
	}

	newpair->next = NULL;

	return newpair;
}

void	ht_free(hashtable_t *hashtable)
{
	int			i;
	entry_t		*ptr;
	entry_t		*next;

	i = hashtable->size;
	while (i < hashtable->size)
	{
		if (!(ptr = hashtable->table[i++]))
			continue ;
		while (ptr != NULL && ptr->key != NULL) {
			next = ptr->next;
			free(ptr->key);
			free(ptr->value);
			free(ptr);
			ptr = next;
		}
	}
	free(hashtable->table);
	free(hashtable);
}

void	ht_clear(hashtable_t *hashtable)
{
	int			i;
	entry_t		*ptr;
	entry_t		*next;

	i = hashtable->size;
	while (i < hashtable->size)
	{
		if (!(ptr = hashtable->table[i++]))
			continue ;
		next = ptr->next;
		free(ptr->key);
		ptr->key = NULL;
		free(ptr->value);
		ptr->value = NULL;
		ptr->next = NULL;

		ptr = next;
		while (ptr != NULL && ptr->key != NULL) {
			next = ptr->next;
			free(ptr->key);
			free(ptr->value);
			free(ptr);
			ptr = next;
		}
	}
}

/* Insert a key-value pair into a hash table. */
void ht_set( hashtable_t *hashtable, char *key, void *value ) {
	int bin = 0;
	entry_t *newpair = NULL;
	entry_t *next = NULL;
	entry_t *last = NULL;

	bin = ht_hash( hashtable, key );

	next = hashtable->table[ bin ];

	while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
		last = next;
		next = next->next;
	}

	/* There's already a pair.  Let's replace that string. */
	if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {

		free( next->value );
		next->value = value;

	/* Nope, could't find it.  Time to grow a pair. */
	} else {
		newpair = ht_newpair( key, value );

		/* We're at the start of the linked list in this bin. */
		if( next == hashtable->table[ bin ] ) {
			newpair->next = next;
			hashtable->table[ bin ] = newpair;
	
		/* We're at the end of the linked list in this bin. */
		} else if ( next == NULL ) {
			last->next = newpair;
	
		/* We're in the middle of the list. */
		} else  {
			newpair->next = next;
			last->next = newpair;
		}
	}
}

/* Retrieve a key-value pair from a hash table. */
void 	*ht_get( hashtable_t *hashtable, char *key ) {
	int bin = 0;
	entry_t *pair;

	bin = ht_hash( hashtable, key );

	/* Step through the bin, looking for our value. */
	pair = hashtable->table[ bin ];
	while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
		pair = pair->next;
	}

	/* Did we actually find anything? */
	if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
		return NULL;

	} else {
		return pair->value;
	}
}

void			ht_remove( hashtable_t *hashtable, char *key )
{
	int bin = 0;
	entry_t *pair;
	entry_t *old;

	bin = ht_hash( hashtable, key );

	/* Step through the bin, looking for our value. */
	pair = hashtable->table[ bin ];
	old = NULL;
	while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
		old = pair;
		pair = pair->next;
	}

	/* Did we actually find anything? */
	if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
		return ;

	} else {
		if (old)
			old->next = pair->next;
		free(pair->key);
		free(pair->value);
		free(pair);
		pair = NULL;
	}
}