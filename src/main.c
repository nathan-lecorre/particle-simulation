#include "distribution.h"
#include "environnement.h"
#include "particules.h"
#include "interface.h"

int main() {
    env e = create_environnement(10, 600, 300, 10, 0.2);

    for (int i = 0 ; i<10 ; i++)
        move_particules(e);

    particule p = get_particule(e, 5);
    get_x(p); get_y(p); get_vx(p); get_vy(p);
    set_speed(p, 5, 1);
    move(p, float 0.4);
    
    char* path
    animate(e,path,get_w(e),get_h(e),10);
    free_environnement(e);
    return 0;
}