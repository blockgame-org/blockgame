#include <blockgame/utility.h>

#include <blockgame/panic.h>

#include <string.h>

void *
bg_calloc(size_t count, size_t size)
{
  void *p = calloc(count, size);

  if (!p && size)
    bg_panic("Allocation error");

  return p;
}

void *
bg_realloc(void *p, size_t size)
{
  p = realloc(p, size);

	if (!p && size)
		bg_panic("Reallocation error");

	return p;
}