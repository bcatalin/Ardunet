/*
  WString.cpp - String library for Wiring & Arduino
  Copyright (c) 2009-10 Hernando Barragan.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "ardunetcore/WString.h"

void ICACHE_FLASH_ATTR ultoa(unsigned long n, char*a, int base) {
    unsigned char nBuffer[8*sizeof(long)];
    unsigned int i = 0;
    while (n>0) {
        nBuffer[i++] = n % base;
        n /= base;
    }
    
    if (i==0) {
        a[0] = '0';
    }
    
    unsigned int j=0;
    for (; i>0 ; i--) {
        a[j++] = (char)((nBuffer[i-1]<10) ? ('0'+nBuffer[i-1]) : ('A'+nBuffer[i-1]-10));
    }
}

void ICACHE_FLASH_ATTR ltoa(long n, char*a, int base) {
    if (n<0) {
        n = -n;
        a[0] = '-';
        a = a+1;
    }
    ultoa((unsigned long)n, a, base);
}

void ICACHE_FLASH_ATTR itoa(int n, char*a, int base) {
    ltoa((long) n, a, base);
}

char ICACHE_FLASH_ATTR tolower(char c) {
    if (c>='A' && c<='Z') {
        return c-'A'+'a';
    }
    return c;
}

char ICACHE_FLASH_ATTR toupper(char c) {
    if (c>='a' && c<='z') {
        return c-'a'+'A';
    }
    return c;
}

boolean ICACHE_FLASH_ATTR isspace(char c) {
    return c==' ';
}

String::String( const char *value )
{
  if ( value == NULL )
    value = "";
  getBuffer( _length = strlen( value ) );
  if ( _buffer != NULL )
    strcpy( _buffer, value );
}

ICACHE_FLASH_ATTR String::String( const String &value )
{
  getBuffer( _length = value._length );
  if ( _buffer != NULL )
    strcpy( _buffer, value._buffer );
}

ICACHE_FLASH_ATTR String::String( const char value )
{
  _length = 1;
  getBuffer(1);
  if ( _buffer != NULL ) {
    _buffer[0] = value;
    _buffer[1] = 0;
  }
}

ICACHE_FLASH_ATTR String::String( const unsigned char value )
{
  _length = 1;
  getBuffer(1);
  if ( _buffer != NULL) {
    _buffer[0] = value;
    _buffer[1] = 0;
  }
}

ICACHE_FLASH_ATTR String::String( const int value, const int base )
{
  char buf[33];  
  itoa((signed long)value, buf, base);
  getBuffer( _length = strlen(buf) );
  if ( _buffer != NULL )
    strcpy( _buffer, buf );
}

ICACHE_FLASH_ATTR String::String( const unsigned int value, const int base )
{
  char buf[33];  
  ultoa((unsigned long)value, buf, base);
  getBuffer( _length = strlen(buf) );
  if ( _buffer != NULL )
    strcpy( _buffer, buf );
}

ICACHE_FLASH_ATTR String::String( const long value, const int base )
{
  char buf[33];  
  ltoa(value, buf, base);
  getBuffer( _length = strlen(buf) );
  if ( _buffer != NULL )
    strcpy( _buffer, buf );
}

ICACHE_FLASH_ATTR String::String( const unsigned long value, const int base )
{
  char buf[33];  
  ultoa(value, buf, 10);
  getBuffer( _length = strlen(buf) );
  if ( _buffer != NULL )
    strcpy( _buffer, buf );
}

char ICACHE_FLASH_ATTR String::charAt( unsigned int loc ) const
{
  return operator[]( loc );
}

void ICACHE_FLASH_ATTR String::setCharAt( unsigned int loc, const char aChar )
{
  if(_buffer == NULL) return;
  if(_length > loc) {
    _buffer[loc] = aChar;
  }
}

int ICACHE_FLASH_ATTR String::compareTo( const String &s2 ) const
{
  return strcmp( _buffer, s2._buffer );
}

const String & ICACHE_FLASH_ATTR String::concat( const String &s2 )
{
  return (*this) += s2;
}

const String & ICACHE_FLASH_ATTR String::operator=( const String &rhs )
{
  if ( this == &rhs )
    return *this;

  if ( rhs._length > _length )
  {
    free(_buffer);
    getBuffer( rhs._length );
  }
 
  if ( _buffer != NULL ) {
    _length = rhs._length;
    strcpy( _buffer, rhs._buffer );
  }
  return *this;
}

//const String & String::operator+=( const char aChar )
//{
//  if ( _length == _capacity )
//    doubleBuffer();
//
//  _buffer[ _length++ ] = aChar;
//  _buffer[ _length ] = '\0';
//  return *this;
//}

const String & ICACHE_FLASH_ATTR String::operator+=( const String &other )
{
  _length += other._length;
  if ( _length > _capacity )
  {
    char *temp = _buffer;
    getBuffer( _length );
    if ( _buffer != NULL )
      strcpy( _buffer, temp );
    free(temp);
  }
  if ( _buffer != NULL )
    strcat( _buffer, other._buffer );
  return *this;
}


int ICACHE_FLASH_ATTR String::operator==( const String &rhs ) const
{
  return ( _length == rhs._length && strcmp( _buffer, rhs._buffer ) == 0 );
}

int ICACHE_FLASH_ATTR String::operator!=( const String &rhs ) const
{
  return ( _length != rhs.length() || strcmp( _buffer, rhs._buffer ) != 0 );
}

int ICACHE_FLASH_ATTR String::operator<( const String &rhs ) const
{
  return strcmp( _buffer, rhs._buffer ) < 0;
}

int ICACHE_FLASH_ATTR String::operator>( const String &rhs ) const
{
  return strcmp( _buffer, rhs._buffer ) > 0;
}

int ICACHE_FLASH_ATTR String::operator<=( const String &rhs ) const
{
  return strcmp( _buffer, rhs._buffer ) <= 0;
}

int ICACHE_FLASH_ATTR String::operator>=( const String & rhs ) const
{
  return strcmp( _buffer, rhs._buffer ) >= 0;
}

char & ICACHE_FLASH_ATTR String::operator[]( unsigned int index )
{
  static char dummy_writable_char;
  if (index >= _length || !_buffer) {
    dummy_writable_char = 0;
    return dummy_writable_char;
  }
  return _buffer[ index ];
}

char ICACHE_FLASH_ATTR String::operator[]( unsigned int index ) const
{
  // need to check for valid index, to do later
  return _buffer[ index ];
}

boolean ICACHE_FLASH_ATTR String::endsWith( const String &s2 ) const
{
  if ( _length < s2._length )
    return 0;

  return strcmp( &_buffer[ _length - s2._length], s2._buffer ) == 0;
}

boolean ICACHE_FLASH_ATTR String::equals( const String &s2 ) const
{
  return ( _length == s2._length && strcmp( _buffer,s2._buffer ) == 0 );
}

boolean ICACHE_FLASH_ATTR String::equalsIgnoreCase( const String &s2 ) const
{
  if ( this == &s2 )
    return true; //1;
  else if ( _length != s2._length )
    return false; //0;

  return strcmp(toLowerCase()._buffer, s2.toLowerCase()._buffer) == 0;
}

String ICACHE_FLASH_ATTR String::replace( char findChar, char replaceChar )
{
  if ( _buffer == NULL ) return *this;
  String theReturn = _buffer;
  char* temp = theReturn._buffer;
  while( (temp = strchr( temp, findChar )) != 0 )
    *temp = replaceChar;

  return theReturn;
}

String ICACHE_FLASH_ATTR String::replace( const String& match, const String& replace )
{
  if ( _buffer == NULL ) return *this;
  String temp = _buffer, newString;

  int loc;
  while ( (loc = temp.indexOf( match )) != -1 )
  {
    newString += temp.substring( 0, loc );
    newString += replace;
    temp = temp.substring( loc + match._length );
  }
  newString += temp;  
  return newString;
}

int ICACHE_FLASH_ATTR String::indexOf( char temp ) const
{
  return indexOf( temp, 0 );
}

int ICACHE_FLASH_ATTR String::indexOf( char ch, unsigned int fromIndex ) const
{
  if ( fromIndex >= _length )
    return -1;

  const char* temp = strchr( &_buffer[fromIndex], ch );
  if ( temp == NULL )
    return -1;

  return temp - _buffer;
}

int ICACHE_FLASH_ATTR String::indexOf( const String &s2 ) const
{
  return indexOf( s2, 0 );
}

int ICACHE_FLASH_ATTR String::indexOf( const String &s2, unsigned int fromIndex ) const
{
  if ( fromIndex >= _length )
    return -1;

  const char *theFind = strstr( &_buffer[ fromIndex ], s2._buffer );

  if ( theFind == NULL )
    return -1;

  return theFind - _buffer; // pointer subtraction
}

int ICACHE_FLASH_ATTR String::lastIndexOf( char theChar ) const
{
  return lastIndexOf( theChar, _length - 1 );
}

int ICACHE_FLASH_ATTR String::lastIndexOf( char ch, unsigned int fromIndex ) const
{
  if ( fromIndex >= _length )
    return -1;

  char tempchar = _buffer[fromIndex + 1];
  _buffer[fromIndex + 1] = '\0';
  char* temp = strrchr( _buffer, ch );
  _buffer[fromIndex + 1] = tempchar;

  if ( temp == NULL )
    return -1;

  return temp - _buffer;
}

int ICACHE_FLASH_ATTR String::lastIndexOf( const String &s2 ) const
{
  return lastIndexOf( s2, _length - s2._length );
}

int ICACHE_FLASH_ATTR String::lastIndexOf( const String &s2, unsigned int fromIndex ) const
{
  // check for empty strings
  if ( s2._length == 0 || s2._length - 1 > fromIndex || fromIndex >= _length )
    return -1;

  // matching first character
  char temp = s2[ 0 ];

  for ( int i = fromIndex; i >= 0; i-- )
  {
    if ( _buffer[ i ] == temp && (*this).substring( i, i + s2._length ).equals( s2 ) )
    return i;
  }
  return -1;
}

boolean ICACHE_FLASH_ATTR String::startsWith( const String &s2 ) const
{
  if ( _length < s2._length )
    return 0;

  return startsWith( s2, 0 );
}

boolean ICACHE_FLASH_ATTR String::startsWith( const String &s2, unsigned int offset ) const
{
  if ( offset > _length - s2._length )
    return 0;

  return strncmp( &_buffer[offset], s2._buffer, s2._length ) == 0;
}

String ICACHE_FLASH_ATTR String::substring( unsigned int left ) const
{
  return substring( left, _length );
}

String ICACHE_FLASH_ATTR String::substring( unsigned int left, unsigned int right ) const
{
  if ( left > right )
  {
    int temp = right;
    right = left;
    left = temp;
  }

  if ( right > _length )
  {
    right = _length;
  }

  char temp = _buffer[ right ];  // save the replaced character
  _buffer[ right ] = '\0';      
  String outPut = ( _buffer + left );  // pointer arithmetic
  _buffer[ right ] = temp;  //restore character
  return outPut;
}

String ICACHE_FLASH_ATTR String::toLowerCase() const
{
  String temp = _buffer;

  for ( unsigned int i = 0; i < _length; i++ )
    temp._buffer[ i ] = (char)tolower( temp._buffer[ i ] );
  return temp;
}

String ICACHE_FLASH_ATTR String::toUpperCase() const
{
  String temp = _buffer;

  for ( unsigned int i = 0; i < _length; i++ )
    temp._buffer[ i ] = (char)toupper( temp._buffer[ i ] );
  return temp;
}

String ICACHE_FLASH_ATTR String::trim() const
{
  if ( _buffer == NULL ) return *this;
  String temp = _buffer;
  unsigned int i,j;

  for ( i = 0; i < _length; i++ )
  {
    if ( !isspace(_buffer[i]) )
      break;
  }

  for ( j = temp._length - 1; j > i; j-- )
  {
    if ( !isspace(_buffer[j]) )
      break;
  }

  return temp.substring( i, j + 1);
}

void ICACHE_FLASH_ATTR String::getBytes(unsigned char *buf, unsigned int bufsize)
{
  if (!bufsize || !buf) return;
  unsigned int len = bufsize - 1;
  if (len > _length) len = _length;
  strncpy((char *)buf, _buffer, len);
  buf[len] = 0;
}

void ICACHE_FLASH_ATTR String::toCharArray(char *buf, unsigned int bufsize)
{
  if (!bufsize || !buf) return;
  unsigned int len = bufsize - 1;
  if (len > _length) len = _length;
  strncpy(buf, _buffer, len);
  buf[len] = 0;
}

char* ICACHE_FLASH_ATTR String::getChars() {
  return _buffer;
}