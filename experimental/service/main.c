#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "dx/core.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static Core_Boolean g_run = Core_True;
static Core_Boolean g_error = Core_False;

static BOOL WINAPI
signalHandler
  (
    _In_ DWORD dwCtrlType
  )
{
  switch (dwCtrlType) {
    case CTRL_C_EVENT: /*0*/{
       fprintf(stdout, "receive ctrl-c event\n");
       g_run = Core_False;
    } break;
    case CTRL_BREAK_EVENT: /*1*/{
       fprintf(stdout, "received break event\n");
       g_run = Core_False;
    } break;
    case CTRL_CLOSE_EVENT: /*2*/{
       fprintf(stdout, "received close event\n");
       g_run = Core_False;
    } break;
    case CTRL_LOGOFF_EVENT: /*5*/{
       fprintf(stdout, "received logoff event\n");
       g_run = Core_False;
    } break;
     case CTRL_SHUTDOWN_EVENT: /*6*/{
       fprintf(stdout, "received shutdown event\n");
       g_run = Core_False;
    } break;
    default: {
      g_error = Core_True;
      g_run = Core_False;
    } break;
  };
  return TRUE;
}

static void added(Core_String** x) {
  CORE_REFERENCE(*x);
}

static void removed(Core_String** x) {
  CORE_UNREFERENCE(*x);
}

static Core_InlineArrayListP g_fileSystemRoots;

#include "FileSystemRoots.h"

int
main
  (
    int argc,
    char** argv
  )
{
  if (Core_initialize()) {
    return EXIT_FAILURE;
  }
#if 1
  {
    Core_InlineArrayListP_Configuration configuration = {
      .addedCallback = (Core_InlineArrayListP_AddedCallback*) & added,
      .removedCallback = (Core_InlineArrayListP_RemovedCallback*) & removed,
    };
    Core_InlineArrayListP_initialize(&g_fileSystemRoots, 0, &configuration);
  }
  if (Core_getFileSystemRoots(&g_fileSystemRoots)) {
    Core_InlineArrayListP_uninitialize(&g_fileSystemRoots);
    Core_uninitialize();
    return EXIT_FAILURE;
  }
#endif
  SetConsoleCtrlHandler(&signalHandler, TRUE);
  while (g_run && Core_Error_NoError != Core_getError()) {
    // Print Universal Coordinate Time (UTC) in ISO 8601 format.
    time_t t1 = time(NULL);
    struct tm* t = gmtime(&t1);
    fprintf(stdout, "%04i-%02i-%02i", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
    fprintf(stdout, "T%02i:%02i:%02i", t->tm_hour, t->tm_min, t->tm_sec);
    fprintf(stdout, "\n");
    // Sleep for one second.
    _sleep(1000);
  }
#if 1
  Core_InlineArrayListP_uninitialize(&g_fileSystemRoots);
#endif
  Core_uninitialize();
  return EXIT_SUCCESS;
}
