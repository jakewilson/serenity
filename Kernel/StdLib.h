/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <AK/Forward.h>
#include <AK/Userspace.h>

namespace Syscall {
struct StringArgument;
}

String copy_string_from_user(const char*, size_t);

extern "C" {

void copy_to_user(void*, const void*, size_t);
void copy_from_user(void*, const void*, size_t);
void memset_user(void*, int, size_t);

void* memcpy(void*, const void*, size_t);
char* strcpy(char*, const char*);
char* strncpy(char*, const char*, size_t);
int strncmp(const char* s1, const char* s2, size_t n);
char* strstr(const char* haystack, const char* needle);
int strcmp(char const*, const char*);
size_t strlen(const char*);
size_t strnlen(const char*, size_t);
void* memset(void*, int, size_t);
int memcmp(const void*, const void*, size_t);
char* strrchr(const char* str, int ch);
void* memmove(void* dest, const void* src, size_t n);
const void* memmem(const void* haystack, size_t, const void* needle, size_t);

inline u16 ntohs(u16 w) { return (w & 0xff) << 8 | ((w >> 8) & 0xff); }
inline u16 htons(u16 w) { return (w & 0xff) << 8 | ((w >> 8) & 0xff); }
}

template<typename T>
inline void copy_from_user(T* dest, const T* src)
{
    copy_from_user(dest, src, sizeof(T));
}

template<typename T>
inline void copy_to_user(T* dest, const T* src)
{
    copy_to_user(dest, src, sizeof(T));
}


template<typename T>
inline void copy_from_user(T* dest, Userspace<const T*> src)
{
    copy_from_user(dest, src.unsafe_userspace_ptr(), sizeof(T));
}

template<typename T>
inline void copy_to_user(Userspace<T*> dest, const T* src)
{
    copy_to_user(dest.unsafe_userspace_ptr(), src, sizeof(T));
}

template<typename T>
inline void copy_to_user(Userspace<T*> dest, const void* src, size_t size)
{
    copy_to_user(dest.unsafe_userspace_ptr(), src, size);
}

template<typename T>
inline void copy_from_user(void* dest, Userspace<const T*> src, size_t size)
{
    copy_from_user(dest, src.unsafe_userspace_ptr(), size);
}
