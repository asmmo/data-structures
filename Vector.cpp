#include <algorithm>

template <typename T>
class Vector
{
    size_t theSize;
    size_t theCapacity;
    T* objects;

public:
    explicit Vector( size_t initSize = 0 ) : theSize{ initSize }, theCapacity{ initSize + SPARE_CAPACITY } { objects = new T[ theCapacity ]; }

    Vector( const Vector & rhs ) : theSize{ rhs.theSize }, theCapacity{ rhs.theCapacity }, objects{ nullptr } {

        objects = new T[ theCapacity ];
        for( size_t k = 0; k < theSize; ++k )
        objects[ k ] = rhs.objects[ k ];
    }

    Vector & operator= ( const Vector & rhs )
    {
        Vector copy{ rhs };
        std::swap( *this, copy );
        return *this;
    }


    Vector( Vector && rhs ) : theSize{ rhs.theSize }, theCapacity{ rhs.theCapacity }, objects{ rhs.objects } {
        rhs.objects = nullptr;    //size and capacity =0 aren't impo
        }

    Vector & operator= ( Vector && rhs )
    {
        std::swap( theSize, rhs.theSize );
        std::swap( theCapacity, rhs.theCapacity );
        std::swap( objects, rhs.objects );
        return *this;
    }


    ~Vector( )  { delete [ ] objects; }


    void resize( size_t newSize )
    {
        if( newSize > theCapacity ) reserve( newSize * 2 );
        theSize = newSize;
    }


    void reserve( size_t newCapacity )
    {
        if( newCapacity < theSize )
            return;

        T*newArray = new T[ newCapacity ];
        for( size_t k = 0; k < theSize; ++k )
            newArray[ k ] = std::move( objects[ k ] );

        theCapacity = newCapacity;
        std::swap( objects, newArray );
        delete [ ] newArray;
    }


    T & operator[]( size_t index )     { return objects[ index ]; }

    const T & operator[]( size_t index ) const { return objects[ index ]; }

    bool empty( ) const { return size( ) == 0; }
    size_t size( ) const { return theSize; }
    size_t capacity( ) const { return theCapacity; }

    void push_back( const T & x )
    {
        if( theSize == theCapacity ) reserve( 2 * theCapacity + 1 );
        objects[ theSize++ ] = x;
    }

    void push_back( T && x )
    {
        if( theSize == theCapacity )    reserve( 2 * theCapacity + 1 );
        objects[ theSize++ ] = std::move( x );
    }

    void pop_back( ) { --theSize; }
    const T & back ( ) const { return objects[ theSize - 1 ]; }


    typedef T* iterator;
    typedef const T* const_iterator;

    iterator begin( ) { return &objects[ 0 ]; }
    const_iterator begin( ) const { return &objects[ 0 ]; }
    iterator end( ) { return &objects[ size( ) ]; }
    const_iterator end( ) const { return &objects[ size( ) ]; }
    
    static const size_t SPARE_CAPACITY = 16;

};
