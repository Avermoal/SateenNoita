#include "EngineCore/scene.h"

#include <glad/glad.h>

#include "EngineCore/window.h"

void initscene(struct scene *scn, strcut GLFWwindow *win)
{
  createtilemap(&scn->map, win);
}

void destroyscene(struct scene *scn)
{

}

void renderscene(struct scene *scn, struct window *win)
{
  
}

void updatescene(struct scene *scn)
{
  
}
