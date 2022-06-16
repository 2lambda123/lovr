#include "util.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#pragma once

struct Blob;
struct Image;

typedef struct {
  uint32_t blob;
  size_t offset;
  size_t size;
  size_t stride;
  char* data;
} ModelBuffer;

typedef enum {
  ATTR_POSITION,
  ATTR_NORMAL,
  ATTR_TEXCOORD,
  ATTR_COLOR,
  ATTR_TANGENT,
  ATTR_BONES,
  ATTR_WEIGHTS,
  MAX_DEFAULT_ATTRIBUTES
} DefaultAttribute;

typedef enum { I8, U8, I16, U16, I32, U32, F32 } AttributeType;

typedef union {
  void* raw;
  int8_t* i8;
  uint8_t* u8;
  int16_t* i16;
  uint16_t* u16;
  int32_t* i32;
  uint32_t* u32;
  float* f32;
} AttributeData;

typedef struct {
  uint32_t offset;
  uint32_t buffer;
  uint32_t count;
  AttributeType type;
  unsigned components : 3;
  unsigned normalized : 1;
  unsigned matrix : 1;
  unsigned hasMin : 1;
  unsigned hasMax : 1;
  float min[4];
  float max[4];
} ModelAttribute;

typedef enum {
  DRAW_POINTS,
  DRAW_LINES,
  DRAW_LINE_LOOP,
  DRAW_LINE_STRIP,
  DRAW_TRIANGLES,
  DRAW_TRIANGLE_STRIP,
  DRAW_TRIANGLE_FAN
} DrawMode;

typedef struct {
  ModelAttribute* attributes[MAX_DEFAULT_ATTRIBUTES];
  ModelAttribute* indices;
  DrawMode mode;
  uint32_t material;
} ModelPrimitive;

typedef enum {
  SCALAR_METALNESS,
  SCALAR_ROUGHNESS,
  SCALAR_ALPHA_CUTOFF,
  MAX_MATERIAL_SCALARS
} MaterialScalar;

typedef enum {
  COLOR_BASE,
  COLOR_EMISSIVE,
  MAX_MATERIAL_COLORS
} MaterialColor;

typedef enum {
  TEXTURE_COLOR,
  TEXTURE_EMISSIVE,
  TEXTURE_METALNESS,
  TEXTURE_ROUGHNESS,
  TEXTURE_OCCLUSION,
  TEXTURE_NORMAL,
  MAX_MATERIAL_TEXTURES
} MaterialTexture;

typedef struct {
  const char* name;
  float metalness;
  float roughness;
  float alphaCutoff;
  float uvShift[2];
  float uvScale[2];
  float baseColor[4];
  float emissiveColor[4];
  uint32_t colorTexture;
  uint32_t emissiveTexture;
  uint32_t metalnessRoughnessTexture;
  uint32_t occlusionTexture;
  uint32_t normalTexture;
} ModelMaterial;

typedef enum {
  PROP_TRANSLATION,
  PROP_ROTATION,
  PROP_SCALE,
} AnimationProperty;

typedef enum {
  SMOOTH_STEP,
  SMOOTH_LINEAR,
  SMOOTH_CUBIC
} SmoothMode;

typedef struct {
  uint32_t nodeIndex;
  AnimationProperty property;
  SmoothMode smoothing;
  uint32_t keyframeCount;
  float* times;
  float* data;
} ModelAnimationChannel;

typedef struct {
  const char* name;
  ModelAnimationChannel* channels;
  uint32_t channelCount;
  float duration;
} ModelAnimation;

typedef struct {
  uint32_t* joints;
  uint32_t jointCount;
  float* inverseBindMatrices;
} ModelSkin;

typedef struct {
  const char* name;
  union {
    float matrix[16];
    struct {
      float translation[4];
      float rotation[4];
      float scale[4];
    } properties;
  } transform;
  uint32_t* children;
  uint32_t childCount;
  uint32_t primitiveIndex;
  uint32_t primitiveCount;
  uint32_t skin;
  bool matrix;
} ModelNode;

typedef struct ModelData {
  uint32_t ref;
  void* data;

  struct Blob** blobs;
  struct Image** images;
  ModelBuffer* buffers;
  ModelAttribute* attributes;
  ModelPrimitive* primitives;
  ModelMaterial* materials;
  ModelAnimation* animations;
  ModelSkin* skins;
  ModelNode* nodes;
  uint32_t rootNode;

  uint32_t blobCount;
  uint32_t imageCount;
  uint32_t bufferCount;
  uint32_t attributeCount;
  uint32_t primitiveCount;
  uint32_t materialCount;
  uint32_t animationCount;
  uint32_t skinCount;
  uint32_t nodeCount;

  ModelAnimationChannel* channels;
  uint32_t* children;
  uint32_t* joints;
  char* chars;
  uint32_t channelCount;
  uint32_t childCount;
  uint32_t jointCount;
  uint32_t charCount;

  map_t animationMap;
  map_t materialMap;
  map_t nodeMap;
} ModelData;

typedef void* ModelDataIO(const char* filename, size_t* bytesRead);

ModelData* lovrModelDataCreate(struct Blob* blob, ModelDataIO* io);
ModelData* lovrModelDataInitGltf(ModelData* model, struct Blob* blob, ModelDataIO* io);
ModelData* lovrModelDataInitObj(ModelData* model, struct Blob* blob, ModelDataIO* io);
ModelData* lovrModelDataInitStl(ModelData* model, struct Blob* blob, ModelDataIO* io);
void lovrModelDataDestroy(void* ref);
void lovrModelDataAllocate(ModelData* model);
