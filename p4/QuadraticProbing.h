        #ifndef _QUADRATIC_PROBING_H_
        #define _QUADRATIC_PROBING_H_

        #include <iostream>
        #include "vector.h"
        using namespace std;

        // QuadraticProbing Hash table class
        //
        // CONSTRUCTION: an initialization for ITEM_NOT_FOUND
        //               and an approximate initial size or default of 101
        //
        // ******************PUBLIC OPERATIONS*********************
        // void insert( key, x )       --> Insert x
        // void remove( key )       --> Remove x
        // Hashable find( key )     --> Return item that matches x
        // void makeEmpty( )      --> Remove all items
        // int hash( String str, int tableSize )
        //                        --> Static method to hash strings

        template <class HashedObj>
        class QuadraticHashTable
        {
          public:
            explicit QuadraticHashTable( const HashedObj & notFound, int size);
            QuadraticHashTable( const QuadraticHashTable & rhs )
              : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
                array( rhs.array ), currentSize( rhs.currentSize ) { }
            void makeEmpty( );
            const QuadraticHashTable & operator=( const QuadraticHashTable & rhs );
            enum EntryType { ACTIVE, EMPTY, DELETED };
            // *********************changed functions*********************
            void insert( unsigned key, const HashedObj & x );
            void remove( unsigned key);
            const HashedObj & find( unsigned key );
            int getCurrentSize() {return currentSize; };
            int getArraySize() {return array.size(); };
            void refresh();

          private:
            struct HashEntry
            {
                unsigned key;
                HashedObj element;
                EntryType info;

              HashEntry( unsigned k = 0, const HashedObj & e = HashedObj( ), EntryType i = EMPTY )
                  : key( k ), element( e ), info( i ) { }
            };
            vector<HashEntry> array;
            int currentSize;
            const HashedObj ITEM_NOT_FOUND;
            bool isPrime( int n ) const;
            int nextPrime( int n ) const;
            bool isActive( int currentPos ) const;
            int hash( const string & key, int tableSize ) const;
            int hash( int key, int tableSize ) const;
            void rehash();
            
            // *********************changed functions*********************
            int findPos( unsigned key ) const;
        };


        #include "QuadraticProbing.cpp"
        #endif
