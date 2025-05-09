#pragma once
#ifndef STDDEF_H
#define STDDEF_H

#ifdef __cplusplus
extern "C" {
#endif

#define NULL ((void*)0)

typedef unsigned long size_t;
typedef int ssize_t;
typedef int ptrdiff_t;

#ifdef __cplusplus
}
#endif

#endif // STDDEF_H