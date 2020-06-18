//
// Created by Dominic on 6/16/2020.
//

#ifndef HASHMAP2_H
#define HASHMAP2_H
/*
||
|| @file HashMap.h
|| @version 1.0 Beta
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | This library provides a simple interface for storing data with an associate key
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/

#ifndef HASHMAP_H
#define HASHMAP_H

//#include <WProgram.h>
template<typename valType>
class mapEntry{
public:

    const char *key;
    valType value;
};

template<typename valType, size_t capacity>
class HashMap2 {
public:
    HashMap2(){}

    ~HashMap2(){
        for(size_t i = 0; i < internalHead; i++) {
            delete entries[i];
        }
    }

    mapEntry<valType>& operator[](int x){
        return *(entries[x]);
    }

    void set(const char* key, valType value){
        size_t foundIndex = getIndexOf(key);
        if(foundIndex != ((size_t) -1)) { //Index found, modify existing record.
            entries[foundIndex]->value = value;
        } else { //No index found, create one.
            size_t index = internalHead;
            internalHead++;

            entries[index] = new mapEntry<valType>{key, value};
        }
    };

    size_t getIndexOf(const char* key) {
        for(size_t i = 0; i < internalHead; i++) {
            if(strcmp(entries[i]->key, key) == 0) {
                return i;
            }
        }
        return (size_t) -1; //Will hopefully overflow to max val.
    }

    valType getValueOf(const char* key){
        for(size_t i = 0; i < internalHead; i++) {
            if(strcmp(entries[i]->key, key) == 0) {
                return *(entries[i]->value);
            }
        }
        return NULL;
    }

private:
    mapEntry<valType> *entries[capacity];
    size_t internalHead;
};

#endif
#endif