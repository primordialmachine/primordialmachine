/*
load "world"
load "addRoom"
push 0
push 0
load "createVector"
world.addRoom(Center : Vec2(0,0), Size : Vec2(256, 256));
*/

#include "dx/core.h"
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdint.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// Definitions of BooleanValue, StringValue, VoidValue, and Value.
// The definition of StringValue is actually a forward declaration.

typedef bool Core_Interpreter_Boolean;
#define Core_Interpreter_Boolean_True (true)
#define Core_Interpreter_Boolean_False (false)

typedef struct Core_Interpreter_String Core_Interpreter_String;

typedef uint8_t Core_Interpreter_Void;

typedef union Core_Interpreter_Value {
  Core_Interpreter_Boolean booleanValue;
  Core_Interpreter_String* stringValue;
  Core_Interpreter_Void voidValue;
} Core_Interpreter_Value;

#define Core_Interpreter_Tag_Boolean (1)
#define Core_Interpreter_Tag_String (2)
#define Core_Interpreter_Tag_Void (3)

typedef struct Core_Interpreter_TaggedValue {
  uint8_t tag;
  Core_Interpreter_Value value;
} Core_Interpreter_TaggedValue;

void Core_Interpreter_TaggedValue_setBoolean(Core_Interpreter_TaggedValue* value, Core_Interpreter_Boolean booleanValue) {
  value->tag = Core_Interpreter_Tag_Boolean;
  value->value.booleanValue = booleanValue;
}

void Core_Interpreter_TaggedValue_setString(Core_Interpreter_TaggedValue* value, Core_Interpreter_String* stringValue) {
  value->tag = Core_Interpreter_Tag_String;
  value->value.stringValue = stringValue;
}

void Core_Interpreter_TaggedValue_setVoid(Core_Interpreter_TaggedValue* value, Core_Interpreter_Void voidValue) {
  value->tag = Core_Interpreter_Tag_Void;
  value->value.voidValue = 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Core_Interpreter_Stack {
  Core_Interpreter_TaggedValue* values;
  Core_Size size, capacity;
} Core_Interpreter_Stack;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Core_Interpreter_String Core_Interpreter_String;

struct Core_Interpreter_String {
  Core_Interpreter_String* next;
  size_t hashValue;
  size_t numberOfBytes;
  uint8_t bytes[];
};

typedef struct _Core_Interpreter_StringTable {
  Core_Interpreter_String * *buckets;
  Core_Size size, capacity;
} _Core_Interpreter_StringTable;

static _Core_Interpreter_StringTable* g_stringTable = NULL;

typedef struct Core_Interpreter_JumpTarget Core_Interpreter_JumpTarget;

struct Core_Interpreter_JumpTarget {
  Core_Interpreter_JumpTarget* previous;
  jmp_buf environment;
};

static int g_error = 0;

static Core_Interpreter_JumpTarget* g_jumpTargets = NULL;

Core_Interpreter_String* Core_Interpreter_String_create(void *bytes,Core_Size numberOfBytes) {
  if (SIZE_MAX - sizeof(Core_Interpreter_String) < numberOfBytes) {
    fprintf(stderr, "%s:%d: string too long\n", __FILE__, __LINE__);
    g_error = 1;
    longjmp(g_jumpTargets->environment, -1);
  }

  // Check if the string exists.
  Core_Size hashValue = numberOfBytes;
  for (Core_Size i = 0, n = numberOfBytes; i < n; ++i) {
    hashValue = (hashValue << 5) ^ (hashValue >> 3) | (((uint8_t*)bytes)[i]);
  }
  Core_Size hashIndex = hashValue % g_stringTable->capacity;

  Core_Interpreter_String* string = g_stringTable->buckets[hashIndex];
  while (string) {
    if (string->hashValue == hashValue && string->numberOfBytes == numberOfBytes) {
      if (!memcmp(string->bytes, bytes, numberOfBytes)) {
        break;
      }
    }
  }

  // The string does exist. Return the string.
  if (string) {
    return string;
  }

  // The string does not exist. Create the string. Add the string. Return the string.
  if (Core_Memory_allocate(&string, sizeof(Core_Interpreter_String) + numberOfBytes)) {
    fprintf(stderr, "%s:%d: unable to allocate %zu Bytes\n", __FILE__, __LINE__, sizeof(Core_String) + numberOfBytes);
    g_error = 1;
    longjmp(g_jumpTargets->environment, -1);
  }
  Core_Memory_copy(string->bytes, bytes, numberOfBytes);
  string->numberOfBytes = numberOfBytes;
  string->hashValue = hashValue;
  string->next = g_stringTable->buckets[hashIndex];
  g_stringTable->buckets[hashIndex] = string;
  g_stringTable->size++;
  return string;
}

bool Core_Interpreter_String_isEqualTo(Core_Interpreter_String* x, Core_Interpreter_String* y) {
  if (x == y) {
    return true;
  }
  if (x->numberOfBytes != y->numberOfBytes) {
    return false;
  }
  return !memcmp(x->bytes, y->bytes, x->numberOfBytes);
}

static void sample1() {
  fprintf(stdout, "executing sample 1\n");
  fprintf(stdout, "Hello, World!\n");
}

static void sample2() {
  fprintf(stdout, "executing sample 2\n");
  fprintf(stdout, "Hello, World!\n");
}

static int Core_Interpreter_startup() {
  if (Core_Memory_allocate(&g_stringTable, sizeof(_Core_Interpreter_StringTable))) {
    return 1;
  }
  if (Core_Memory_allocate((void**)&g_stringTable->buckets, sizeof(Core_String*) * 8)) {
    Core_Memory_deallocate(g_stringTable);
    g_stringTable = NULL;
    return 1;
  }
  for (Core_Size i = 0, n = 8; i < n; ++i) {
    g_stringTable->buckets[i] = NULL;
  }
  g_stringTable->size = 0;
  g_stringTable->capacity = 8;
  return 0;
}

static void Core_Interpreter_shutdown() {
  if (g_stringTable) {
    for (Core_Size i = 0, n = g_stringTable->capacity; i < n; ++i) {
      Core_Interpreter_String** x = &g_stringTable->buckets[i];
      while (NULL != *x) {
        Core_Interpreter_String* y = *x;
        (*x) = (*x)->next;
        Core_Memory_deallocate(y);
      }
    }
    g_stringTable->size = 0;
    Core_Memory_deallocate(g_stringTable);
    g_stringTable = NULL;
  }
}

int main(int argc, char **argv) {
  if (Core_Interpreter_startup()) {
    return EXIT_FAILURE;
  }
  int exitCode = EXIT_SUCCESS;
  Core_Interpreter_JumpTarget jumpTarget;
  jumpTarget.previous = g_jumpTargets;
  g_jumpTargets = &jumpTarget;
  if (!setjmp(jumpTarget.environment)) {
    if (argc != 2) {
      fprintf(stderr, "too few arguments\n");
      longjmp(jumpTarget.environment, -1);
    }
    Core_Interpreter_String* programName = Core_Interpreter_String_create(argv[1], strlen(argv[1]));
    if (Core_Interpreter_String_isEqualTo(programName, Core_Interpreter_String_create("sample1", sizeof("sample1") - 1))) {
      sample1();
    } else if (Core_Interpreter_String_isEqualTo(programName, Core_Interpreter_String_create("sample2", sizeof("sample2") - 1))) {
      sample2();
    } else {
      fprintf(stderr, "unknown program name %s\n", argv[1]);
      longjmp(jumpTarget.environment, -1);
    }
  } else {
    exitCode = EXIT_FAILURE;
  }
  g_jumpTargets = g_jumpTargets->previous;
  Core_Interpreter_shutdown();
  return exitCode;
}