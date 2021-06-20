#include "../index.h"

int index_convert_negative_safe(int len, int index) {
  if (index < 0) {
    index = len + index;
  }

  if (!index_is_valid(len, index)) {
    exit_error("Invalid index detected", "std/c/index.c",
               "index_convert_negative_safe");
  }

  return index;
}

bool index_is_valid(int len, int index) {
  if (index < 0 || index >= len) {
    return false;
  }

  return true;
}


bool index_closer_to_start(int len, int index) {
  // Determine which end of the list is closer to the index
  bool from_start = true;
  if (index > len / 2) {
    from_start = false;
  }

  return from_start;
}