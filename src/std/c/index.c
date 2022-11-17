#include "../index.h"

bool index_closer_to_start(int len, int index)
{
  // Determine which end of the list is closer to the index
  bool from_start = true;
  if (index > len / 2)
  {
    from_start = false;
  }

  return from_start;
}