#ifndef _wvector_H
#define _wvector_H

/*
 * The next line is used because Codewarrior has a conflict with
 * the STL wvector. Make sure to put the #include of this file
 * AFTER all the system includes.
 */
#define wvector wVector
#include <cstddef> // for NULL

class ArrayIndexOutOfBounds { };

template <class Object>
class wvector
{
  public:
    explicit wvector( int theSize = 0 ) : currentSize( theSize )
      { objects = new Object[ currentSize ]; }
    wvector( const wvector & rhs ) : objects( NULL )
      { operator=( rhs ); }
    ~wvector( )
      { delete [ ] objects; }

    int size( ) const
      { return currentSize; }

    Object & operator[]( int index )
    {
                                                     #ifndef NO_CHECK
        if( index < 0 || index >= currentSize )
            throw ArrayIndexOutOfBounds( );
                                                     #endif
        return objects[ index ];
    }

    const Object & operator[]( int index ) const
    {
                                                     #ifndef NO_CHECK
        if( index < 0 || index >= currentSize )
            throw ArrayIndexOutOfBounds( );
                                                     #endif
        return objects[ index ];
    }


    const wvector & operator = ( const wvector & rhs );
    void resize( int newSize );
  private:
    int currentSize;
    Object * objects;
};

#include "wvector.cpp"
#endif


