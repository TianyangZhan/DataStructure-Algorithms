#include "QuadraticProbing.h"
#include <iostream>
using namespace std;


        /**
         * Internal method to test if a positive number is prime.
         * Not an efficient algorithm.
         */
        template <class HashedObj>
        bool QuadraticHashTable<HashedObj>::isPrime( int n ) const
        {
            if( n == 2 || n == 3 )
                return true;

            if( n == 1 || n % 2 == 0 )
                return false;

            for( int i = 3; i * i <= n; i += 2 )
                if( n % i == 0 )
                    return false;

            return true;
        }

        /**
         * Internal method to return a prime number at least as large as n.
         * Assumes n > 0.
         */
        template <class HashedObj>
        int QuadraticHashTable<HashedObj>::nextPrime( int n ) const
        {
            if( n % 2 == 0 )
                n++;

            for( ; !isPrime( n ); n += 2 )
                ;

            return n;
        }

        /**
         * Construct the hash table.
         */
        template <class HashedObj>
        QuadraticHashTable<HashedObj>::QuadraticHashTable( const HashedObj & notFound, int size )
          : array( nextPrime( size ) ), ITEM_NOT_FOUND( notFound )
        {
            makeEmpty( );
        }


        /**
         * Insert item x into the hash table. If the item is
         * already present, then do nothing.
         */
        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::insert(unsigned key, const HashedObj & x)
        {
          int currentPos = findPos(key);
          if( !isActive(currentPos) ){
              array[currentPos] = HashEntry(key,x,ACTIVE);
              if( ++currentSize > array.size( ) - 49 )
                  rehash( );
          }
                //        if(currentSize > maxSize){maxSize = currentSize;}
        }

        /**
         * Remove item x from the hash table.
         */
        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::remove( unsigned key)
        {
            int currentPos = findPos(key);
                if( isActive(currentPos) ){
                    array[currentPos].info = DELETED;
                }
        }


        /**
         * Expand the hash table.
         */
        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::refresh( )
        {
            vector<HashEntry> oldArray = array;
            
            // Create new double-sized, empty table
            array.resize( oldArray.size( ) );   // changed from 2 to 1.05
            for( int j = 0; j < array.size(); j++ )
                array[ j ].info = EMPTY;
            
            // Copy table over
            currentSize = 0;
            //cout<<"maxhash/hashed"<< maxSize <<"  type"<<ITEM_NOT_FOUND <<endl;
            for( int i = 0; i < oldArray.size( ); i++ )
                if( oldArray[ i ].info == ACTIVE )
                    insert( oldArray[ i ].key, oldArray[ i ].element );
        }

        /**
         * Expand the hash table.
         */
        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::rehash( )
        {
            vector<HashEntry> oldArray = array;

                // Create new double-sized, empty table
            array.resize( nextPrime( 1.05 * oldArray.size( ) ) );   // changed from 2 to 1.05
            for( int j = 0; j < array.size(); j++ )
                array[ j ].info = EMPTY;

                // Copy table over
            currentSize = 0;
            //cout<<"maxhash/hashed"<< maxSize <<"  type"<<ITEM_NOT_FOUND <<endl;
            for( int i = 0; i < oldArray.size( ); i++ )
                if( oldArray[ i ].info == ACTIVE )
                    insert( oldArray[ i ].key, oldArray[ i ].element );
        }


        template <class HashedObj>
        int QuadraticHashTable<HashedObj>::findPos( unsigned key ) const
        {
          /* 1*/      int collisionNum = 0;
          /* 2*/      int currentPos = hash(key, array.size( ) );
          
          /* 3*/      while( array[ currentPos ].info != EMPTY &&
                            array[ currentPos ].key != key )
          {
            
            /* 4*/          currentPos += 2 * ++collisionNum - 1;  // Compute ith probe
            /* 5*/          if( currentPos >= array.size( ) )
            /* 6*/              currentPos -= array.size( );
          }
          
          /* 7*/      return currentPos;
        }


        /**
         * Find item x in the hash table.
         * Return the matching item, or ITEM_NOT_FOUND, if not found.
         */
        template <class HashedObj>
        const HashedObj & QuadraticHashTable<HashedObj>::find( unsigned key )
        {
            int currentPos = findPos(key);
            return isActive(currentPos) ? array[ currentPos ].element : ITEM_NOT_FOUND;
        }

        /**
         * Make the hash table logically empty.
         */
        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::makeEmpty( )
        {
            currentSize = 0;
            //maxSize = 0;
            for( int i = 0; i < array.size( ); i++ )
                array[ i ].info = EMPTY;
        }

        /**
         * Deep copy.
         */
        template <class HashedObj>
        const QuadraticHashTable<HashedObj> & QuadraticHashTable<HashedObj>::
        operator=( const QuadraticHashTable<HashedObj> & rhs )
        {
            if( this != &rhs )
            {
                array = rhs.array;
                currentSize = rhs.currentSize;
                //maxSize = rhs.maxSize;
            }
            return *this;
        }


        /**
         * Return true if currentPos exists and is active.
         */
        template <class HashedObj>
        bool QuadraticHashTable<HashedObj>::isActive( int currentPos ) const
        {
            return array[ currentPos ].info == ACTIVE;
        }


        /**
         * A hash routine for string objects.
         */
        template <class HashedObj>
        int QuadraticHashTable<HashedObj>::hash( const string & key, int tableSize ) const
        {
            int hashVal = 0;
            
            for( int i = 0; i < key.length( ); i++ )
                hashVal = 37 * hashVal + key[ i ];
            
            hashVal %= tableSize;
            if( hashVal < 0 )
                hashVal += tableSize;
            
            return hashVal;
        }

        /**
         * A hash routine for ints.
         */
         template <class HashedObj>
        int QuadraticHashTable<HashedObj>::hash( int key, int tableSize ) const
        {
            if( key < 0 ) key = -key;
            return key % tableSize;
        }
