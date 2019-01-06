#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdint.h>
#include <stdbool.h>

typedef void (* timer_cb)(void);


typedef struct
{
    
    bool (* post)(const char *msg, const char *args);
    
    bool (* createTimer)(timer_cb cb, uint32_t delay_ms, bool periodic);
    
} registry_t;


typedef struct {
    const char *name;
    uint16_t id;
    
    bool (* initialize) (const registry_t *reg);
    bool (* start) (void);
    bool (* stop) (void);

    
    bool (* post)(const char *msg, const char *args);
    
} component_t;

#endif // COMPONENT_H

