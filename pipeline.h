#ifndef PIPELINE_H
#define PIPELINE_H

#include <stddef.h>

// Sample struct input type
typedef struct {
    int value;
} PipelineData;

// Filter function type with typed input
typedef void (*FilterFunc)(PipelineData* data, void* context);

typedef struct Pipeline Pipeline;

Pipeline* pipeline_create(size_t initial_capacity);
int pipeline_add_filter(Pipeline* p, FilterFunc func, void* context);
void pipeline_apply(Pipeline* p, PipelineData* input);
void pipeline_destroy(Pipeline* p);

#endif
