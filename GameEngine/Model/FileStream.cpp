#include "FileStream.h"
#include <cstring>

void binary_buffer_create(Binary_buffer_pointer buffer, uint64_t alloc_size) {
    buffer->_byte_array =  (uint8_t*)malloc(alloc_size);
    buffer->_bytes = alloc_size;
    buffer->_bytes_off = 0;
}

void binary_buffer_destroy(Binary_buffer_pointer buffer) {
    free(buffer->_byte_array);
}

uint8_t binary_buffer_pop8(Binary_buffer_pointer buffer) {
    if(buffer->_bytes < buffer->_bytes_off + 1) return 0;
    return buffer->_byte_array[buffer->_bytes_off++];
}

uint16_t binary_buffer_pop16(Binary_buffer_pointer buffer) {
    if(buffer->_bytes < buffer->_bytes_off + 2) return 0;
    return (buffer->_byte_array[buffer->_bytes_off++])
           | (buffer->_byte_array[buffer->_bytes_off++] << 8);
}

uint32_t binary_buffer_pop32(Binary_buffer_pointer buffer) {
    if(buffer->_bytes < buffer->_bytes_off + 4) return 0;
    return (buffer->_byte_array[buffer->_bytes_off++] << 0)
           | (buffer->_byte_array[buffer->_bytes_off++] << 8)
           | (buffer->_byte_array[buffer->_bytes_off++] << 16)
           | (buffer->_byte_array[buffer->_bytes_off++] << 24);
}

uint64_t binary_buffer_pop64(Binary_buffer_pointer buffer) {
    if(buffer->_bytes < buffer->_bytes_off + 8) return 0;
    return (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 0))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 8))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 16))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 24))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 32))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 40))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 48))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 56));
}

float binary_buffer_pop_float(Binary_buffer_pointer buffer) {
    float f32;
    memcpy(&f32, buffer->_byte_array + buffer->_bytes_off, 4);
    buffer->_bytes_off += 4;
    return f32;
}

const char* binary_buffer_pop_string(Binary_buffer_pointer buffer, uint64_t string_size) {
    char* _binary_string_buffer = (char*)malloc(string_size);
    memcpy(_binary_string_buffer, &buffer->_byte_array[buffer->_bytes_off], string_size);
    return _binary_string_buffer;
}

const char* binary_buffer_pop_string_with_termination(Binary_buffer_pointer buffer, uint64_t string_size) {
    char* _binary_string_buffer = (char*)malloc(string_size + 1);
    memcpy(_binary_string_buffer, &buffer->_byte_array[buffer->_bytes_off], string_size);
    _binary_string_buffer[string_size] = '\0';
    buffer->_bytes_off += string_size;
    return _binary_string_buffer;
}

void binary_buffer_push8(Binary_buffer_pointer buffer, uint8_t u8) {
    if(buffer->_bytes_off + 1 > buffer->_bytes) {
        uint8_t stack[buffer->_bytes];
        memcpy(stack, buffer->_byte_array, buffer->_bytes);
        buffer->_byte_array = (uint8_t*)realloc(buffer->_byte_array, buffer->_bytes + 1);
        memcpy(buffer->_byte_array, stack, buffer->_bytes);
        buffer->_bytes += 1;
    }
    buffer->_byte_array[buffer->_bytes_off++] = u8;
}

void binary_buffer_push16(Binary_buffer_pointer buffer, uint16_t u16) {
    if(buffer->_bytes_off + 2 > buffer->_bytes) {
        uint8_t stack[buffer->_bytes];
        memcpy(stack, buffer->_byte_array, buffer->_bytes);
        buffer->_byte_array = (uint8_t*)realloc(buffer->_byte_array, buffer->_bytes + 2);
        memcpy(buffer->_byte_array, stack, buffer->_bytes);
        buffer->_bytes += 2;
    }
    buffer->_byte_array[buffer->_bytes_off++] = ((u16 & 0x00FF));
    buffer->_byte_array[buffer->_bytes_off++] = ((u16 & 0xFF00) << 8);
}

void binary_buffer_push32(Binary_buffer_pointer buffer, uint32_t u32) {
    if(buffer->_bytes_off + 4 > buffer->_bytes) {
        uint8_t stack[buffer->_bytes];
        memcpy(stack, buffer->_byte_array, buffer->_bytes);
        buffer->_byte_array = (uint8_t*)realloc(buffer->_byte_array, buffer->_bytes + 4);
        memcpy(buffer->_byte_array, stack, buffer->_bytes);
        buffer->_bytes += 4;
    }
    buffer->_byte_array[buffer->_bytes_off++] = ((u32 & 0x000000FF));
    buffer->_byte_array[buffer->_bytes_off++] = ((u32 & 0x0000FF00) << 8);
    buffer->_byte_array[buffer->_bytes_off++] = ((u32 & 0x00FF0000) << 16);
    buffer->_byte_array[buffer->_bytes_off++] = ((u32 & 0xFF000000) << 24);
}

void binary_buffer_push64(Binary_buffer_pointer buffer, uint64_t u64) {
    if (buffer->_bytes_off + 8 > buffer->_bytes) {
        uint8_t stack[buffer->_bytes];
        memcpy(stack, buffer->_byte_array, buffer->_bytes);
        buffer->_byte_array = (uint8_t *) realloc(buffer->_byte_array, buffer->_bytes + 8);
        memcpy(buffer->_byte_array, stack, buffer->_bytes);
        buffer->_bytes += 8;
    }
    buffer->_byte_array[buffer->_bytes_off++] = ((u64 & 0x00000000000000FF));
    buffer->_byte_array[buffer->_bytes_off++] = ((u64 & 0x000000000000FF00) >> 8);
    buffer->_byte_array[buffer->_bytes_off++] = ((u64 & 0x0000000000FF0000) >> 16);
    buffer->_byte_array[buffer->_bytes_off++] = ((u64 & 0x00000000FF000000) >> 24);
    buffer->_byte_array[buffer->_bytes_off++] = ((u64 & 0x000000FF00000000) >> 32);
    buffer->_byte_array[buffer->_bytes_off++] = ((u64 & 0x0000FF0000000000) >> 40);
    buffer->_byte_array[buffer->_bytes_off++] = ((u64 & 0x00FF000000000000) >> 48);
    buffer->_byte_array[buffer->_bytes_off++] = ((u64 & 0xFF00000000000000) >> 56);
}

void binary_buffer_push_float(Binary_buffer_pointer buffer, float f32) {
    if (buffer->_bytes_off + 4 > buffer->_bytes) {
        uint8_t stack[buffer->_bytes];
        memcpy(stack, buffer->_byte_array, buffer->_bytes);
        buffer->_byte_array = (uint8_t *) realloc(buffer->_byte_array, buffer->_bytes + 4);
        memcpy(buffer->_byte_array, stack, buffer->_bytes);
        buffer->_bytes += 4;
    }
    memcpy(buffer->_byte_array + buffer->_bytes_off, &f32, sizeof(float));
    buffer->_bytes_off += 4;
}

void binary_buffer_push_string(Binary_buffer_pointer buffer, const char* string) {
    uint64_t _string_len = strlen(string);
    if (buffer->_bytes_off + _string_len > buffer->_bytes) {
        uint8_t stack[buffer->_bytes];
        memcpy(stack, buffer->_byte_array, buffer->_bytes);
        buffer->_byte_array = (uint8_t *) realloc(buffer->_byte_array, buffer->_bytes + _string_len);
        memcpy(buffer->_byte_array, stack, buffer->_bytes);
        buffer->_bytes += _string_len;
    }
    memcpy(buffer->_byte_array + buffer->_bytes_off, string, _string_len);
    buffer->_bytes_off += _string_len;
}

typedef const char* Path;

void binary_buffer_read_from_file(Binary_buffer_pointer buffer, Path file_path) {
    FILE* _f = fopen(file_path, "rb");
    if(_f == NULL) {
        buffer->_byte_array = NULL;
        return;
    }

    fseek(_f, 0, SEEK_END);
    uint64_t _len = ftell(_f);
    rewind(_f);

    buffer->_byte_array = (uint8_t*)malloc(_len);
    if(buffer->_byte_array == NULL) {
        return;
    }
    buffer->_bytes = _len;
    buffer->_bytes_off = 0;

    fread(buffer->_byte_array, 1, _len, _f);

    fclose(_f);
}

void binary_buffer_write_to_file(Binary_buffer_pointer buffer, Path file_path) {
    FILE *_f = fopen(file_path, "wb");
    if (_f == NULL) {
        buffer->_byte_array = NULL;
        return;
    }

    fwrite(buffer->_byte_array, 1, buffer->_bytes_off, _f);
    fclose(_f);
}

bool contains(const char* searchFor, const char* searchIn) {
    uint64_t forlen = strlen(searchFor);
    uint64_t inLen = strlen(searchIn);
    for(uint64_t i =  0; i < inLen; i ++) {
        if(searchIn[i] == searchFor[0]) {
            for(uint64_t j = 0; j < forlen; j++) {
                if(searchIn[i + j] != searchFor[j]) {
                    break;
                } else {
                    if(j == forlen - 1) return true;
                }
            }
        }
    }
    return false;
}

char* getFilename(char* filename) {
    int len = strlen(filename);
    char* lastSlash = filename;
    for (int i = 0; i < len; i++) {
        if (filename[i] == '/' || filename[i] == '\\') {
            lastSlash = filename + i + 1;
        }
    }
    return lastSlash;
}

const char* toLow(const char* string ) {
    uint64_t len = strlen(string);
    char* stringLow = (char*)malloc(len * sizeof(char));
    for(uint64_t i = 0; i < len; i++)
        stringLow[i] = (char)tolower((int)string[i]);
    stringLow[len] = '\0';
    return stringLow;
}