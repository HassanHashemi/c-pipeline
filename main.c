#include "pipeline.h"
#include <stdio.h>

void increment(PipelineData* data, void* ctx) {
    data->value += 1;
}

void scale(PipelineData* data, void* ctx) {
    int factor = ctx ? *(int*)ctx : 1;
    data->value *= factor;
}

int main() {
    PipelineData d = { .value = 10 };
    int factor = 5;

    Pipeline* p = pipeline_create(2);
    pipeline_add_filter(p, increment, NULL);
    pipeline_add_filter(p, scale, &factor);

    pipeline_apply(p, &d);

    printf("result: %d\n", d.value);

    pipeline_destroy(p);
    return 0;
}
