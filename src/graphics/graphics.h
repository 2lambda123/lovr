#include "graphics/canvas.h"
#include "graphics/font.h"
#include "graphics/material.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "math/math.h"
#include "util.h"
#include <stdbool.h>
#include <stdint.h>

#pragma once

#define MAX_TRANSFORMS 64
#define MAX_PIPELINES 16

typedef void (*StencilCallback)(void* userdata);

typedef enum {
  ARC_MODE_PIE,
  ARC_MODE_OPEN,
  ARC_MODE_CLOSED
} ArcMode;

typedef enum {
  BARRIER_ALL,
  BARRIER_BLOCKS,
  BARRIER_IMAGES,
  BARRIER_TEXTURES
} Barrier;

typedef enum {
  BLEND_ALPHA,
  BLEND_ADD,
  BLEND_SUBTRACT,
  BLEND_MULTIPLY,
  BLEND_LIGHTEN,
  BLEND_DARKEN,
  BLEND_SCREEN,
  BLEND_REPLACE
} BlendMode;

typedef enum {
  BLEND_ALPHA_MULTIPLY,
  BLEND_PREMULTIPLIED
} BlendAlphaMode;

typedef enum {
  COMPARE_NONE,
  COMPARE_EQUAL,
  COMPARE_NEQUAL,
  COMPARE_LESS,
  COMPARE_LEQUAL,
  COMPARE_GREATER,
  COMPARE_GEQUAL
} CompareMode;

typedef enum {
  DRAW_MODE_FILL,
  DRAW_MODE_LINE
} DrawMode;

typedef enum {
  STENCIL_REPLACE,
  STENCIL_INCREMENT,
  STENCIL_DECREMENT,
  STENCIL_INCREMENT_WRAP,
  STENCIL_DECREMENT_WRAP,
  STENCIL_INVERT
} StencilAction;

typedef enum {
  WINDING_CLOCKWISE,
  WINDING_COUNTERCLOCKWISE
} Winding;

typedef struct {
  bool computeShaders;
  bool writableBlocks;
} GraphicsFeatures;

typedef struct {
  bool initialized;
  float pointSizes[2];
  int textureSize;
  int textureMSAA;
  float textureAnisotropy;
} GraphicsLimits;

typedef struct {
  int shaderSwitches;
  int drawCalls;
} GraphicsStats;

typedef struct {
  bool stereo;
  Canvas* canvas;
  float viewport[2][4];
  float viewMatrix[2][16];
  float projection[2][16];
} Camera;

typedef struct {
  Color backgroundColor;
  BlendMode blendMode;
  BlendAlphaMode blendAlphaMode;
  Canvas* canvas[MAX_CANVASES];
  int canvasCount;
  Color color;
  bool culling;
  CompareMode depthTest;
  bool depthWrite;
  Font* font;
  float lineWidth;
  float pointSize;
  Shader* shader;
  CompareMode stencilMode;
  int stencilValue;
  Winding winding;
  bool wireframe;
} Pipeline;

typedef struct {
  Mesh* mesh;
  MeshDrawMode mode;
  struct {
    uint32_t count;
    float* data;
  } vertex;
  struct {
    uint32_t count;
    uint16_t* data;
  } index;
  struct {
    int start;
    int count;
  } range;
  DefaultShader shader;
  Material* material;
  Texture* textures[MAX_MATERIAL_TEXTURES];
  mat4 transform;
  int instances;
} DrawOptions;

typedef struct {
  Mesh* mesh;
  Shader* shader;
  Material* material;
  Camera camera;
  float transform[16];
  Pipeline pipeline;
  int instances;
} DrawCommand;

typedef struct {
  bool initialized;
  bool gammaCorrect;
  int msaa;
  void* window;
  Camera camera;
  Shader* defaultShaders[MAX_DEFAULT_SHADERS];
  Material* defaultMaterial;
  Font* defaultFont;
  Mesh* defaultMesh;
  TextureFilter defaultFilter;
  float transforms[MAX_TRANSFORMS][16];
  int transform;
  Pipeline pipelines[MAX_PIPELINES];
  int pipeline;
} GraphicsState;

// Base
void lovrGraphicsInit();
void lovrGraphicsDestroy();
void lovrGraphicsPresent();
void lovrGraphicsCreateWindow(int w, int h, bool fullscreen, int msaa, const char* title, const char* icon);
void lovrGraphicsGetDimensions(int* width, int* height);
int lovrGraphicsGetMSAA();
void lovrGraphicsSetCamera(Camera* camera, bool clear);
GraphicsFeatures lovrGraphicsGetSupported();
GraphicsLimits lovrGraphicsGetLimits();
GraphicsStats lovrGraphicsGetStats();

// State
void lovrGraphicsReset();
void lovrGraphicsPushPipeline();
void lovrGraphicsPopPipeline();
Color lovrGraphicsGetBackgroundColor();
void lovrGraphicsSetBackgroundColor(Color color);
void lovrGraphicsGetBlendMode(BlendMode* mode, BlendAlphaMode* alphaMode);
void lovrGraphicsSetBlendMode(BlendMode mode, BlendAlphaMode alphaMode);
void lovrGraphicsGetCanvas(Canvas** canvas, int* count);
void lovrGraphicsSetCanvas(Canvas** canvas, int count);
Color lovrGraphicsGetColor();
void lovrGraphicsSetColor(Color color);
bool lovrGraphicsIsCullingEnabled();
void lovrGraphicsSetCullingEnabled(bool culling);
TextureFilter lovrGraphicsGetDefaultFilter();
void lovrGraphicsSetDefaultFilter(TextureFilter filter);
void lovrGraphicsGetDepthTest(CompareMode* mode, bool* write);
void lovrGraphicsSetDepthTest(CompareMode depthTest, bool write);
Font* lovrGraphicsGetFont();
void lovrGraphicsSetFont(Font* font);
bool lovrGraphicsIsGammaCorrect();
void lovrGraphicsSetGammaCorrect(bool gammaCorrect);
float lovrGraphicsGetLineWidth();
void lovrGraphicsSetLineWidth(float width);
float lovrGraphicsGetPointSize();
void lovrGraphicsSetPointSize(float size);
Shader* lovrGraphicsGetShader();
void lovrGraphicsSetShader(Shader* shader);
void lovrGraphicsGetStencilTest(CompareMode* mode, int* value);
void lovrGraphicsSetStencilTest(CompareMode mode, int value);
Winding lovrGraphicsGetWinding();
void lovrGraphicsSetWinding(Winding winding);
bool lovrGraphicsIsWireframe();
void lovrGraphicsSetWireframe(bool wireframe);

// Transforms
void lovrGraphicsPush();
void lovrGraphicsPop();
void lovrGraphicsOrigin();
void lovrGraphicsTranslate(float x, float y, float z);
void lovrGraphicsRotate(float angle, float ax, float ay, float az);
void lovrGraphicsScale(float x, float y, float z);
void lovrGraphicsMatrixTransform(mat4 transform);

// Rendering
VertexPointer lovrGraphicsGetVertexPointer(uint32_t capacity);
void lovrGraphicsClear(Color* color, float* depth, int* stencil);
void lovrGraphicsDraw(DrawOptions* draw);
void lovrGraphicsPoints(uint32_t count);
void lovrGraphicsLine(uint32_t count);
void lovrGraphicsTriangle(DrawMode mode, Material* material, float points[9]);
void lovrGraphicsPlane(DrawMode mode, Material* material, mat4 transform);
void lovrGraphicsBox(DrawMode mode, Material* material, mat4 transform);
void lovrGraphicsArc(DrawMode mode, ArcMode, Material* material, mat4 transform, float theta1, float theta2, int segments);
void lovrGraphicsCircle(DrawMode mode, Material* material, mat4 transform, int segments);
void lovrGraphicsCylinder(Material* material, float x1, float y1, float z1, float x2, float y2, float z2, float r1, float r2, bool capped, int segments);
void lovrGraphicsSphere(Material* material, mat4 transform, int segments);
void lovrGraphicsSkybox(Texture* texture, float angle, float ax, float ay, float az);
void lovrGraphicsPrint(const char* str, mat4 transform, float wrap, HorizontalAlign halign, VerticalAlign valign);
void lovrGraphicsStencil(StencilAction action, int replaceValue, StencilCallback callback, void* userdata);
void lovrGraphicsFill(Texture* texture);
#define lovrGraphicsCompute lovrGpuCompute
#define lovrGraphicsWait lovrGpuWait

// GPU

typedef void (*gpuProc)(void);

void lovrGpuInit(bool srgb, gpuProc (*getProcAddress)(const char*));
void lovrGpuDestroy();
void lovrGpuClear(Canvas** canvas, int canvasCount, Color* color, float* depth, int* stencil);
void lovrGpuDraw(DrawCommand* command);
void lovrGpuCompute(Shader* shader, int x, int y, int z);
void lovrGpuWait(int barriers);
void lovrGpuPresent();

void lovrGpuBindTexture(Texture* texture, int slot);
void lovrGpuDirtyTexture(int slot);
