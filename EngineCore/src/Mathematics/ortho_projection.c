#include "Mathemathics/orthoprojection.h"

void create_ortho_projection(float projection[16], float left, float right, float bottom, float top)
{
  projection[0] = 2.0f/(right-left);
  projection[1] = 0.0f;
  projection[2] = 0.0f;
  projection[3] = 0.0f;

  projection[4] = 0.0f;
  projection[5] = 2.0f/(top-bottom);
  projection[6] = 0.0f;
  projection[7] = 0.0f;

  projection[8] = 0.0f;
  projection[9] = 0.0f;
  projection[10] = -1.0f;
  projection[11] = 0.0f;
  
  projection[12] = -(rigth+left)/(right-left);
  projection[13] = -(top+bottom)/(top-bottom);
  projection[14] = 0.0f;
  projection[15] = 1.0f;
}
