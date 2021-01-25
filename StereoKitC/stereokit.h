#pragma once

#define SK_32BIT_INDICES
// #define SK_NO_FLATSCREEN
// #define SK_NO_LEAP_MOTION
// #define SK_NO_RUNTIME_SHADER_COMPILE

#define SK_VERSION_MAJOR 0
#define SK_VERSION_MINOR 2
#define SK_VERSION_PATCH 1
#define SK_VERSION_PRERELEASE 0

#if defined(_DLL)
#define SK_EXIMPORT dllexport
#else
#define SK_EXIMPORT dllimport
#endif

#ifdef __cplusplus
#define SK_API extern "C" __declspec(SK_EXIMPORT)
#define SK_API_S __declspec(SK_EXIMPORT)
#define sk_default(x) = x
#define sk_ref(x) x&
#define sk_ref_arr(x) x*&
#define SK_MakeFlag(enumType) \
inline enumType  operator| (enumType  a, enumType b)        { return static_cast<enumType>(static_cast<int>(a) | static_cast<int>(b)); } \
inline enumType &operator|=(enumType& a, const enumType& b) { a = a | b; return a; } \
inline enumType  operator& (enumType  a, enumType b)        { return static_cast<enumType>(static_cast<int>(a) & static_cast<int>(b)); } \
inline enumType &operator&=(enumType& a, const enumType& b) { a = a & b; return a; } \
inline enumType  operator~ (const enumType& a)              { return static_cast<enumType>(~static_cast<int>(a)); }
#else
#define SK_API __declspec(SK_EXIMPORT)
#define SK_API_S __declspec(SK_EXIMPORT)
#define sk_default(x)
#define sk_ref(x) x*
#define sk_ref_arr(x) x**
#define SK_MakeFlag(enumType)
#endif

#define SK_DeclarePrivateType(name) struct _ ## name; typedef struct _ ## name *name;

#include <stdint.h>
#include <math.h>

#ifdef __cplusplus
namespace sk {
#endif

typedef int32_t bool32_t;

///////////////////////////////////////////

typedef enum runtime_ {
	runtime_flatscreen   = 0,
	runtime_mixedreality = 1
} runtime_;

typedef struct settings_t {
	int32_t flatscreen_pos_x;
	int32_t flatscreen_pos_y;
	int32_t flatscreen_width;
	int32_t flatscreen_height;
	char assets_folder[128];
	bool32_t disable_flatscreen_mr_sim;
} settings_t;

typedef enum display_ {
	display_opaque = 0,
	display_additive,
	display_passthrough,
} display_;

typedef struct system_info_t {
	display_ display_type;
	int32_t  display_width;
	int32_t  display_height;
	bool32_t spatial_bridge;
} system_info_t;

SK_API bool32_t      sk_init          (const char *app_name, runtime_ preferred_runtime, bool32_t fallback sk_default(true) );
SK_API void          sk_shutdown      ();
SK_API void          sk_quit          ();
SK_API bool32_t      sk_step          (void (*app_update)(void));
SK_API runtime_      sk_active_runtime();
SK_API void          sk_set_settings  (const sk_ref(settings_t) settings);
SK_API system_info_t sk_system_info   ();
SK_API const char   *sk_version_name  ();
SK_API uint64_t      sk_version_id    ();

///////////////////////////////////////////

SK_API float    time_getf_unscaled();
SK_API double   time_get_unscaled ();
SK_API float    time_getf();
SK_API double   time_get ();
SK_API float    time_elapsedf_unscaled();
SK_API double   time_elapsed_unscaled ();
SK_API float    time_elapsedf();
SK_API double   time_elapsed ();
SK_API void     time_scale(double scale);
SK_API void     time_set_time(double total_seconds, double frame_elapsed_seconds sk_default(0));

///////////////////////////////////////////

typedef struct vec2 {
	float x, y;
} vec2;
typedef struct vec3 {
	float x, y, z;
} vec3;
typedef struct vec4 {
	float x, y, z, w;
} vec4;
typedef struct rect {
	float x, y, w, h;
} rect;
typedef struct quat {
	float x, y, z, w;
} quat;
typedef union matrix {
	vec4 row[4];
	float m[16];
} matrix;
typedef struct ray_t {
	vec3 pos;
	vec3 dir;
} ray_t;
typedef struct bounds_t {
	vec3 center;
	vec3 dimensions;
} bounds_t;
typedef struct plane_t {
	vec3  normal;
	float d;
} plane_t;
typedef struct sphere_t {
	vec3  center;
	float radius;
} sphere_t;
typedef struct pose_t {
	vec3 position;
	quat orientation;
} pose_t;

SK_API vec3 vec3_cross(const sk_ref(vec3) a, const sk_ref(vec3) b);

SK_API quat quat_difference (const sk_ref(quat) a, const sk_ref(quat) b);
SK_API quat quat_lookat     (const sk_ref(vec3) from, const sk_ref(vec3) at);
SK_API quat quat_lookat_up  (const sk_ref(vec3) from, const sk_ref(vec3) at, const sk_ref(vec3) up);
SK_API quat quat_from_angles(float pitch_x_deg, float yaw_y_deg, float roll_z_deg);
SK_API quat quat_slerp      (const sk_ref(quat) a, const sk_ref(quat) b, float t);
SK_API quat quat_normalize  (const sk_ref(quat) a);
SK_API quat quat_inverse    (const sk_ref(quat) a);
SK_API quat quat_mul        (const sk_ref(quat) a, const sk_ref(quat) b);
SK_API vec3 quat_mul_vec    (const sk_ref(quat) a, const sk_ref(vec3) b);

SK_API matrix pose_matrix(const sk_ref(pose_t) pose, vec3 scale sk_default((vec3{1,1,1})));
SK_API void   pose_matrix_out(const sk_ref(pose_t) pose, sk_ref(matrix) out_result, vec3 scale sk_default((vec3{1,1,1})));

SK_API void   matrix_inverse      (const sk_ref(matrix) a, sk_ref(matrix) out_matrix);
SK_API void   matrix_mul          (const sk_ref(matrix) a, const sk_ref(matrix) b, sk_ref(matrix) out_matrix);
SK_API vec3   matrix_mul_point    (const sk_ref(matrix) transform, const sk_ref(vec3) point);
SK_API vec3   matrix_mul_direction(const sk_ref(matrix) transform, const sk_ref(vec3) direction);
SK_API quat   matrix_mul_rotation (const sk_ref(matrix)  transform, const sk_ref(quat) orientation);
SK_API matrix matrix_trs          (const sk_ref(vec3) position, const sk_ref(quat) orientation sk_default((quat{0,0,0,1})), const sk_ref(vec3) scale sk_default((vec3{1,1,1})));
SK_API void   matrix_trs_out      (sk_ref(matrix) out_result, const sk_ref(vec3) position, const sk_ref(quat) orientation sk_default((quat{0,0,0,1})), const sk_ref(vec3) scale sk_default((vec3{1,1,1})));

SK_API bool32_t ray_intersect_plane(ray_t ray, vec3 plane_pt, vec3 plane_normal, sk_ref(float) out_t);
SK_API bool32_t ray_from_mouse     (vec2 screen_pixel_pos, sk_ref(ray_t) out_ray);

#ifdef __cplusplus
static inline vec2  operator*(const vec2 &a, const float b) { return { a.x * b, a.y * b }; }
static inline vec2  operator/(const vec2 &a, const float b) { return { a.x / b, a.y / b }; }
static inline vec2  operator+(const vec2 &a, const vec2 &b) { return { a.x + b.x, a.y + b.y }; }
static inline vec2  operator-(const vec2 &a, const vec2 &b) { return { a.x - b.x, a.y - b.y }; }
static inline vec2  operator*(const vec2 &a, const vec2 &b) { return { a.x * b.x, a.y * b.y }; }
static inline vec2  operator/(const vec2 &a, const vec2 &b) { return { a.x / b.x, a.y / b.y }; }
static inline vec2& operator+=(vec2& a, const vec2& b)     { a.x += b.x; a.y += b.y; return a; }
static inline vec2& operator-=(vec2& a, const vec2& b)     { a.x -= b.x; a.y -= b.y; return a; }
static inline vec2& operator*=(vec2& a, const float b)     { a.x *= b; a.y *= b; return a; }
static inline vec2& operator/=(vec2& a, const float b)     { a.x /= b; a.y /= b; return a; }

static inline vec3  operator*(const vec3 &a, const float b) { return { a.x * b, a.y * b, a.z * b }; }
static inline vec3  operator/(const vec3 &a, const float b) { return { a.x / b, a.y / b, a.z / b }; }
static inline vec3  operator+(const vec3 &a, const vec3 &b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
static inline vec3  operator-(const vec3 &a, const vec3 &b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
static inline vec3  operator-(const vec3 &a)                { return { -a.x, -a.y, -a.z }; }
static inline vec3  operator*(const vec3 &a, const vec3 &b) { return { a.x * b.x, a.y * b.y, a.z * b.z }; }
static inline vec3  operator/(const vec3 &a, const vec3 &b) { return { a.x / b.x, a.y / b.y, a.z / b.z }; }
static inline vec3& operator+=(vec3& a, const vec3& b)     { a.x += b.x; a.y += b.y; a.z += b.z; return a; }
static inline vec3& operator-=(vec3& a, const vec3& b)     { a.x -= b.x; a.y -= b.y; a.z -= b.z; return a; }
static inline vec3& operator*=(vec3& a, const float b)     { a.x *= b; a.y *= b; a.z *= b; return a; }
static inline vec3& operator/=(vec3& a, const float b)     { a.x /= b; a.y /= b; a.z /= b; return a; }

static inline vec3   operator*(const quat &a, const vec3 &b) { return quat_mul_vec(a, b); }
static inline quat   operator*(const quat &a, const quat &b) { return quat_mul(a, b); }
static inline matrix operator*(const sk_ref(matrix) a, const sk_ref(matrix) b) { matrix result; matrix_mul(a, b, result); return result; }

static inline float vec3_magnitude_sq(const sk_ref(vec3) a) { return a.x * a.x + a.y * a.y + a.z * a.z; }
static inline float vec3_magnitude   (const sk_ref(vec3) a) { return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z); }
static inline vec3  vec3_normalize   (const sk_ref(vec3) a) { return a / vec3_magnitude(a); }
static inline vec3  vec3_lerp        (const sk_ref(vec3) a, const sk_ref(vec3) b, float t) { return a + (b - a)*t; }
static inline float vec3_dot         (const sk_ref(vec3) a, const sk_ref(vec3) b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
static inline vec3  vec3_abs         (const sk_ref(vec3) a) { return { fabsf(a.x), fabsf(a.y), fabsf(a.z) }; }

static inline vec2  vec2_lerp        (const sk_ref(vec2) a, const sk_ref(vec2) b, float t) { return a + (b - a)*t; }
#else
static inline float vec3_magnitude_sq(const sk_ref(vec3) a) { return a->x * a->x + a->y * a->y + a->z * a->z; }
static inline float vec3_magnitude   (const sk_ref(vec3) a) { return sqrtf(a->x * a->x + a->y * a->y + a->z * a->z); }
static inline vec3  vec3_normalize   (const sk_ref(vec3) a) { float mag = 1.0f/vec3_magnitude(a); vec3 result = {a->x*mag, a->y*mag, a->z*mag}; return result; }
static inline vec3  vec3_lerp        (const sk_ref(vec3) a, const sk_ref(vec3) b, float t) { vec3 result = { a->x + (b->x - a->x)*t, a->y + (b->y - a->y)*t, a->z + (b->z - a->z)*t }; return result; }
static inline float vec3_dot         (const sk_ref(vec3) a, const sk_ref(vec3) b) { return a->x*b->x + a->y*b->y + a->z*b->z; }
static inline vec3  vec3_abs         (const sk_ref(vec3) a) { vec3 result = { fabsf(a->x), fabsf(a->y), fabsf(a->z) }; return result; }

static inline vec2  vec2_lerp        (const sk_ref(vec2) a, const sk_ref(vec2) b, float t) { vec2 result = {a->x + (b->x - a->x)*t, a->y + (b->y - a->y)*t}; return result; }
#endif

static const float deg2rad = 0.01745329252f;
static const float rad2deg = 57.295779513f;
static const float cm2m = 0.01f;
static const float mm2m = 0.001f;
static const float m2cm = 100.f;
static const float m2mm = 1000.f;

static const vec2 vec2_zero     = { 0,0 };
static const vec2 vec2_one      = { 1,1 };
static const vec3 vec3_one      = { 1,1, 1 };
static const vec3 vec3_zero     = { 0,0, 0 };
static const vec3 vec3_up       = { 0,1, 0 };
static const vec3 vec3_forward  = { 0,0,-1 };
static const vec3 vec3_right    = { 1,0, 0 };
static const quat quat_identity = { 0,0, 0,1 };
static const matrix matrix_identity = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

#define unit_cm(cm) ((cm)*0.01f)
#define unit_mm(mm) ((mm)*0.001f)
#define unit_dmm(dmm, distance) ((dmm)*(distance))

///////////////////////////////////////////

SK_API bool32_t plane_ray_intersect  (plane_t plane, ray_t ray, vec3 *out_pt);
SK_API bool32_t plane_line_intersect (plane_t plane, vec3 p1, vec3 p2, vec3 *out_pt);
SK_API vec3     plane_point_closest  (plane_t plane, vec3 pt);
SK_API bool32_t sphere_ray_intersect (sphere_t sphere, ray_t ray, vec3 *out_pt);
SK_API bool32_t sphere_point_contains(sphere_t sphere, vec3 pt);
SK_API bool32_t bounds_ray_intersect (bounds_t bounds, ray_t ray, vec3 *out_pt);
SK_API bool32_t bounds_point_contains(bounds_t bounds, vec3 pt);
SK_API bool32_t bounds_line_contains (bounds_t bounds, vec3 pt1, vec3 pt2);
SK_API vec3     ray_point_closest    (ray_t ray, vec3 pt);

///////////////////////////////////////////

typedef struct color32 {
	uint8_t r, g, b, a;
} color32;
typedef struct color128 {
	float r, g, b, a;
} color128;

SK_API color128 color_hsv   (float hue, float saturation, float value, float transparency);
SK_API vec3     color_to_hsv(const sk_ref(color128) color);
SK_API color128 color_lab   (float l, float a, float b, float transparency);
SK_API vec3     color_to_lab(const sk_ref(color128) color);

#ifdef __cplusplus
static inline color128  operator*(const color128 &a, const float b) { return { a.r * b, a.g * b, a.b * b, a.a * b }; }
inline color128 color_lerp  (const color128 &a, const sk_ref(color128) b, float t) { return {a.r + (b.r - a.r)*t, a.g + (b.g - a.g)*t, a.b + (b.b - a.b)*t, a.a + (b.a - a.a)*t}; }
inline color32  color_to_32 (const color128 &a) { return {(uint8_t)(a.r * 255.f), (uint8_t)(a.g * 255.f), (uint8_t)(a.b * 255.f), (uint8_t)(a.a * 255.f)}; }
#else
inline color128 color_lerp  (const color128 *a, const sk_ref(color128) b, float t) { color128 result = {a->r + (b->r - a->r)*t, a->g + (b->g - a->g)*t, a->b + (b->b - a->b)*t, a->a + (b->a - a->a)*t}; return result; }
inline color32  color_to_32 (const color128 *a) { color32 result = {(uint8_t)(a->r * 255.f), (uint8_t)(a->g * 255.f), (uint8_t)(a->b * 255.f), (uint8_t)(a->a * 255.f)}; return result; }
#endif

///////////////////////////////////////////

typedef struct gradient_key_t {
	color128 color;
	float    position;
} gradient_key_t;
SK_DeclarePrivateType(gradient_t);

SK_API gradient_t gradient_create ();
SK_API gradient_t gradient_create_keys(const gradient_key_t *keys, int32_t count);
SK_API void       gradient_add    (gradient_t gradient, color128 color, float position);
SK_API void       gradient_set    (gradient_t gradient, int32_t index, color128 color, float position);
SK_API void       gradient_remove (gradient_t gradient, int32_t index);
SK_API int32_t    gradient_count  (gradient_t gradient);
SK_API color128   gradient_get    (gradient_t gradient, float at);
SK_API color32    gradient_get32  (gradient_t gradient, float at);
SK_API void       gradient_release(gradient_t gradient);

///////////////////////////////////////////

typedef struct spherical_harmonics_t {
	vec3 coefficients[9];
} spherical_harmonics_t;
typedef struct sh_light_t {
	vec3     dir_to;
	color128 color;
} sh_light_t;

SK_API spherical_harmonics_t sh_create(const sh_light_t* lights, int32_t light_count);
SK_API color128              sh_lookup(const sk_ref(spherical_harmonics_t)  lookup, vec3 normal);

///////////////////////////////////////////

typedef struct vert_t {
	vec3    pos;
	vec3    norm;
	vec2    uv;
	color32 col;
} vert_t;

#ifdef SK_32BIT_INDICES
typedef uint32_t vind_t;
#else
typedef uint16_t vind_t;
#endif

SK_DeclarePrivateType(mesh_t);

SK_API mesh_t   mesh_find         (const char *name);
SK_API mesh_t   mesh_create       ();
SK_API void     mesh_set_id       (mesh_t mesh, const char *id);
SK_API void     mesh_release      (mesh_t mesh);
SK_API void     mesh_set_keep_data(mesh_t mesh, bool32_t keep_data);
SK_API bool32_t mesh_get_keep_data(mesh_t mesh);
SK_API void     mesh_set_verts    (mesh_t mesh, vert_t *vertices,      int32_t vertex_count, bool32_t calculate_bounds sk_default(true));
SK_API void     mesh_get_verts    (mesh_t mesh, sk_ref_arr(vert_t) out_vertices, sk_ref(int32_t) out_vertex_count);
SK_API void     mesh_set_inds     (mesh_t mesh, vind_t *indices,       int32_t index_count);
SK_API void     mesh_get_inds     (mesh_t mesh, sk_ref_arr(vind_t) out_indices,  sk_ref(int32_t) out_index_count);
SK_API void     mesh_set_draw_inds(mesh_t mesh, int32_t index_count);
SK_API void     mesh_set_bounds   (mesh_t mesh, const sk_ref(bounds_t) bounds);
SK_API bounds_t mesh_get_bounds   (mesh_t mesh);
SK_API bool32_t mesh_ray_intersect(mesh_t mesh, ray_t model_space_ray, vec3 *out_pt);

SK_API mesh_t mesh_gen_plane       (vec2 dimensions, vec3 plane_normal, vec3 plane_top_direction, int32_t subdivisions sk_default(0));
SK_API mesh_t mesh_gen_cube        (vec3 dimensions, int32_t subdivisions sk_default(0));
SK_API mesh_t mesh_gen_sphere      (float diameter,  int32_t subdivisions sk_default(4));
SK_API mesh_t mesh_gen_rounded_cube(vec3 dimensions, float edge_radius, int32_t subdivisions);
SK_API mesh_t mesh_gen_cylinder    (float diameter,  float depth, vec3 direction, int32_t subdivisions sk_default(16));

///////////////////////////////////////////

typedef enum tex_type_ {
	tex_type_image_nomips  = 1 << 0,
	tex_type_cubemap       = 1 << 1,
	tex_type_rendertarget  = 1 << 2,
	tex_type_depth         = 1 << 3,
	tex_type_mips          = 1 << 4,
	tex_type_dynamic       = 1 << 5,
	tex_type_image         = tex_type_image_nomips | tex_type_mips,
} tex_type_;
SK_MakeFlag(tex_type_);

typedef enum tex_format_ {
	tex_format_none = 0,
	tex_format_rgba32,
	tex_format_rgba32_linear,
	tex_format_rgba64,
	tex_format_rgba128,
	tex_format_r8,
	tex_format_r16,
	tex_format_r32,
	tex_format_depthstencil,
	tex_format_depth32,
	tex_format_depth16,
} tex_format_;

typedef enum tex_sample_ {
	tex_sample_linear = 0,
	tex_sample_point,
	tex_sample_anisotropic
} tex_sample_;

typedef enum tex_address_ {
	tex_address_wrap = 0,
	tex_address_clamp,
	tex_address_mirror,
} tex_address_;

SK_DeclarePrivateType(tex_t);

SK_API tex_t tex_find                (const char *id);
SK_API tex_t tex_create              (tex_type_ type sk_default(tex_type_image), tex_format_ format sk_default(tex_format_rgba32));
SK_API tex_t tex_create_mem          (void *data, size_t data_size,       bool32_t srgb_data sk_default(true));
SK_API tex_t tex_create_file         (const char *file,                   bool32_t srgb_data sk_default(true));
SK_API tex_t tex_create_cubemap_file (const char *equirectangular_file,   bool32_t srgb_data sk_default(true), spherical_harmonics_t *sh_lighting_info sk_default(nullptr));
SK_API tex_t tex_create_cubemap_files(const char **cube_face_file_xxyyzz, bool32_t srgb_data sk_default(true), spherical_harmonics_t *sh_lighting_info sk_default(nullptr));
SK_API void  tex_set_id              (tex_t texture, const char *id);
SK_API void  tex_release             (tex_t texture);
SK_API void  tex_set_colors          (tex_t texture, int32_t width, int32_t height, void *data);
SK_API void  tex_set_color_arr       (tex_t texture, int32_t width, int32_t height, void** data, int32_t data_count, spherical_harmonics_t *sh_lighting_info sk_default(nullptr));
SK_API tex_t tex_add_zbuffer         (tex_t texture, tex_format_ format sk_default(tex_format_depthstencil));
SK_API void  tex_rtarget_clear       (tex_t render_target, color32 color);
SK_API void  tex_rtarget_set_active  (tex_t render_target);
SK_API void  tex_get_data            (tex_t texture, void *out_data, size_t out_data_size);
SK_API void *tex_get_resource        (tex_t texture);
SK_API void  tex_set_resource        (tex_t texture, void *surface);
SK_API tex_t tex_gen_cubemap         (const gradient_t gradient, vec3 gradient_dir, int32_t resolution, spherical_harmonics_t* sh_lighting_info sk_default(nullptr));
SK_API tex_t tex_gen_cubemap_sh      (const sk_ref(spherical_harmonics_t)  lookup, int32_t face_size);
SK_API tex_format_  tex_get_format    (tex_t texture);
SK_API int32_t      tex_get_width     (tex_t texture);
SK_API int32_t      tex_get_height    (tex_t texture);
SK_API void         tex_set_sample    (tex_t texture, tex_sample_ sample sk_default(tex_sample_linear));
SK_API tex_sample_  tex_get_sample    (tex_t texture);
SK_API void         tex_set_address   (tex_t texture, tex_address_ address_mode sk_default(tex_address_wrap));
SK_API tex_address_ tex_get_address   (tex_t texture);
SK_API void         tex_set_anisotropy(tex_t texture, int32_t anisotropy_level sk_default(4));
SK_API int32_t      tex_get_anisotropy(tex_t texture);

///////////////////////////////////////////

SK_DeclarePrivateType(font_t);

SK_API font_t font_find   (const char *id);
SK_API font_t font_create (const char *file);
SK_API void   font_set_id (font_t font, const char* id);
SK_API void   font_release(font_t font);
SK_API tex_t  font_get_tex(font_t font);

///////////////////////////////////////////

SK_DeclarePrivateType(shader_t);

SK_API shader_t    shader_find        (const char *id);
SK_API bool32_t    shader_compile     (const char *hlsl, const char *opt_filename, sk_ref_arr(void) out_data, sk_ref(size_t) out_size);
SK_API shader_t    shader_create_file (const char *filename);
SK_API shader_t    shader_create_hlsl (const char *hlsl);
SK_API shader_t    shader_create_mem  (void *data, size_t data_size);
SK_API void        shader_set_id      (shader_t shader, const char *id);
SK_API const char *shader_get_name    (shader_t shader);
SK_API bool32_t    shader_set_code    (shader_t shader, void *data, size_t data_size);
SK_API bool32_t    shader_set_codefile(shader_t shader, const char *filename);
SK_API void        shader_release     (shader_t shader);

///////////////////////////////////////////

typedef enum transparency_ {
	transparency_none = 1,
	transparency_blend,
	transparency_clip,
} transparency_;
typedef enum cull_ {
	cull_back = 0,
	cull_front,
	cull_none,
} cull_;
typedef enum material_param_ {
	material_param_float = 0,
	material_param_color128,
	material_param_vector,
	material_param_matrix,
	material_param_texture,
} material_param_;

SK_DeclarePrivateType(material_t);

SK_API material_t material_find            (const char *id);
SK_API material_t material_create          (shader_t shader);
SK_API material_t material_copy            (material_t material);
SK_API material_t material_copy_id         (const char *id);
SK_API void       material_set_id          (material_t material, const char *id);
SK_API void       material_release         (material_t material);
SK_API void       material_set_transparency(material_t material, transparency_ mode);
SK_API void       material_set_cull        (material_t material, cull_ mode);
SK_API void       material_set_wireframe   (material_t material, bool32_t wireframe);
SK_API void       material_set_queue_offset(material_t material, int32_t offset);
SK_API void       material_set_float       (material_t material, const char *name, float    value);
SK_API void       material_set_color       (material_t material, const char *name, color128 value);
SK_API void       material_set_vector      (material_t material, const char *name, vec4     value);
SK_API void       material_set_matrix      (material_t material, const char *name, matrix   value);
SK_API bool32_t   material_set_texture     (material_t material, const char *name, tex_t    value);
SK_API bool32_t   material_set_texture_id  (material_t material, uint64_t    id,   tex_t    value);
SK_API bool32_t   material_has_param       (material_t material, const char *name, material_param_ type);
SK_API void       material_set_param       (material_t material, const char *name, material_param_ type, const void *value);
SK_API void       material_set_param_id    (material_t material, uint64_t    id,   material_param_ type, const void *value);
SK_API bool32_t   material_get_param       (material_t material, const char *name, material_param_ type, void *out_value);
SK_API bool32_t   material_get_param_id    (material_t material, uint64_t    id,   material_param_ type, void *out_value);
SK_API void       material_get_param_info  (material_t material, int index, char **out_name, material_param_ *out_type);
SK_API int        material_get_param_count (material_t material);
SK_API void       material_set_shader      (material_t material, shader_t shader);
SK_API shader_t   material_get_shader      (material_t material);

///////////////////////////////////////////

typedef enum text_fit_ {
	text_fit_wrap     = 1 << 0,
	text_fit_clip     = 1 << 1,
	text_fit_squeeze  = 1 << 2,
	text_fit_exact    = 1 << 3,
	text_fit_overflow = 1 << 4
} text_fit_;
typedef enum text_align_ {
	text_align_x_left   = 1 << 0,
	text_align_y_top    = 1 << 1,
	text_align_x_center = 1 << 2,
	text_align_y_center = 1 << 3,
	text_align_x_right  = 1 << 4,
	text_align_y_bottom = 1 << 5,
	text_align_center   = text_align_x_center | text_align_y_center,
} text_align_;
SK_MakeFlag(text_align_);

typedef int32_t text_style_t;

SK_API text_style_t text_make_style(font_t font, float character_height, material_t material, color32 color);
SK_API void         text_add_at    (const char *text, const sk_ref(matrix)  transform, text_style_t style sk_default(-1), text_align_ position sk_default(text_align_x_center | text_align_y_center), text_align_ align sk_default(text_align_x_center | text_align_y_center), float off_x sk_default(0), float off_y sk_default(0), float off_z sk_default(0));
SK_API void         text_add_in    (const char *text, const sk_ref(matrix)  transform, vec2 size, text_fit_ fit, text_style_t style sk_default(-1), text_align_ position sk_default(text_align_x_center | text_align_y_center), text_align_ align sk_default(text_align_x_center | text_align_y_center), float off_x sk_default(0), float off_y sk_default(0), float off_z sk_default(0));
SK_API vec2         text_size      (const char *text, text_style_t style sk_default(-1));

///////////////////////////////////////////

typedef enum solid_type_ {
	solid_type_normal = 0,
	solid_type_immovable,
	solid_type_unaffected,
} solid_type_;

typedef void* solid_t;

SK_API solid_t solid_create          (const sk_ref(vec3) position, const sk_ref(quat) rotation, solid_type_ type sk_default(solid_type_normal));
SK_API void    solid_release         (solid_t solid);
SK_API void    solid_add_sphere      (solid_t solid, float diameter sk_default(1), float kilograms sk_default(1), const vec3 *offset sk_default(nullptr));
SK_API void    solid_add_box         (solid_t solid, const sk_ref(vec3) dimensions,float kilograms sk_default(1), const vec3 *offset sk_default(nullptr));
SK_API void    solid_add_capsule     (solid_t solid, float diameter, float height, float kilograms sk_default(1), const vec3 *offset sk_default(nullptr));
SK_API void    solid_set_type        (solid_t solid, solid_type_ type);
SK_API void    solid_set_enabled     (solid_t solid, bool32_t enabled);
SK_API void    solid_move            (solid_t solid, const sk_ref(vec3) position, const sk_ref(quat) rotation);
SK_API void    solid_teleport        (solid_t solid, const sk_ref(vec3) position, const sk_ref(quat) rotation);
SK_API void    solid_set_velocity    (solid_t solid, const sk_ref(vec3) meters_per_second);
SK_API void    solid_set_velocity_ang(solid_t solid, const sk_ref(vec3) radians_per_second);
SK_API void    solid_get_pose        (const solid_t solid, sk_ref(pose_t) out_pose);

///////////////////////////////////////////

SK_DeclarePrivateType(model_t);

SK_API model_t    model_find         (const char *id);
SK_API model_t    model_create       ();
SK_API model_t    model_create_mesh  (mesh_t mesh, material_t material);
SK_API model_t    model_create_mem   (const char *filename, void *data, size_t data_size, shader_t shader sk_default(nullptr));
SK_API model_t    model_create_file  (const char *filename, shader_t shader sk_default(nullptr));
SK_API void       model_set_id       (model_t model, const char *id);
SK_API void       model_release      (model_t model);
SK_API material_t model_get_material (model_t model, int32_t subset);
SK_API mesh_t     model_get_mesh     (model_t model, int32_t subset);
SK_API matrix     model_get_transform(model_t model, int32_t subset);
SK_API void       model_set_material (model_t model, int32_t subset, material_t material);
SK_API void       model_set_mesh     (model_t model, int32_t subset, mesh_t mesh);
SK_API void       model_set_transform(model_t model, int32_t subset, const sk_ref(matrix) transform);
SK_API void       model_remove_subset(model_t model, int32_t subset);
SK_API int32_t    model_add_subset   (model_t model, mesh_t mesh, material_t material, const sk_ref(matrix) transform);
SK_API int32_t    model_subset_count (model_t model);
SK_API void       model_recalculate_bounds(model_t model);
SK_API void       model_set_bounds   (model_t model, const sk_ref(bounds_t) bounds);
SK_API bounds_t   model_get_bounds   (model_t model);

///////////////////////////////////////////

SK_DeclarePrivateType(sprite_t);

typedef enum sprite_type_ {
	sprite_type_atlased = 0,
	sprite_type_single
} sprite_type_;

SK_API sprite_t sprite_create     (tex_t    sprite,      sprite_type_ type sk_default(sprite_type_atlased), const char *atlas_id sk_default("default"));
SK_API sprite_t sprite_create_file(const char *filename, sprite_type_ type sk_default(sprite_type_atlased), const char *atlas_id sk_default("default"));
SK_API void     sprite_set_id     (sprite_t sprite, const char *id);
SK_API void     sprite_release    (sprite_t sprite);
SK_API float    sprite_get_aspect (sprite_t sprite);
SK_API int32_t  sprite_get_width  (sprite_t sprite);
SK_API int32_t  sprite_get_height (sprite_t sprite);
SK_API vec2     sprite_get_dimensions_normalized(sprite_t sprite);
SK_API void     sprite_draw       (sprite_t sprite, const sk_ref(matrix) transform, color32 color sk_default((color32{255,255,255,255})));

///////////////////////////////////////////

typedef struct line_point_t {
	vec3    pt;
	float   thickness;
	color32 color;
} line_point_t;

SK_API void line_add      (vec3 start, vec3 end, color32 color_start, color32 color_end, float thickness);
SK_API void line_addv     (line_point_t start, line_point_t end);
SK_API void line_add_list (const vec3 *points, int32_t count, color32 color, float thickness);
SK_API void line_add_listv(const line_point_t *points, int32_t count);

///////////////////////////////////////////

SK_API void     render_set_clip      (float near_plane sk_default(0.01f), float far_plane sk_default(50));
SK_API void     render_set_fov       (float field_of_view_degrees sk_default(90.0f));
SK_API matrix   render_get_projection();
SK_API matrix   render_get_cam_root  ();
SK_API void     render_set_cam_root  (const sk_ref(matrix) cam_root);
SK_API void     render_set_skytex    (tex_t sky_texture);
SK_API tex_t    render_get_skytex    ();
SK_API void     render_set_skylight  (const sk_ref(spherical_harmonics_t) light_info);
SK_API void     render_set_clear_color(color32 color);
SK_API void     render_enable_skytex (bool32_t show_sky);
SK_API bool32_t render_enabled_skytex();
SK_API void     render_add_mesh      (mesh_t mesh, material_t material, const sk_ref(matrix) transform, color128 color sk_default((color128{1,1,1,1})));
SK_API void     render_add_model     (model_t model, const sk_ref(matrix) transform, color128 color sk_default((color128{1,1,1,1})));
SK_API void     render_blit          (tex_t to_rendertarget, material_t material);
SK_API void     render_screenshot    (vec3 from_viewpt, vec3 at, int width, int height, const char *file);
SK_API void     render_get_device    (void **device, void **context);

///////////////////////////////////////////

SK_API void     hierarchy_push       (const sk_ref(matrix) transform);
SK_API void     hierarchy_pop        ();
SK_API void     hierarchy_set_enabled(bool32_t enabled);
SK_API bool32_t hierarchy_is_enabled ();
SK_API const sk_ref(matrix) hierarchy_to_world();
SK_API const sk_ref(matrix) hierarchy_to_local();
SK_API vec3     hierarchy_to_local_point    (const sk_ref(vec3) world_pt);
SK_API vec3     hierarchy_to_local_direction(const sk_ref(vec3) world_dir);
SK_API quat     hierarchy_to_local_rotation (const sk_ref(quat) world_orientation);
SK_API vec3     hierarchy_to_world_point    (const sk_ref(vec3) local_pt);
SK_API vec3     hierarchy_to_world_direction(const sk_ref(vec3) local_dir);
SK_API quat     hierarchy_to_world_rotation (const sk_ref(quat) local_orientation);

///////////////////////////////////////////

SK_DeclarePrivateType(sound_t);

SK_API sound_t sound_find    (const char *id);
SK_API void    sound_set_id  (sound_t sound, const char *id);
SK_API sound_t sound_create  (const char *filename);
SK_API sound_t sound_generate(float (*function)(float), float duration);
SK_API void    sound_play    (sound_t sound, vec3 at, float volume);
SK_API void    sound_release (sound_t sound);

///////////////////////////////////////////

typedef enum input_source_ {
	input_source_any        = 0x7FFFFFFF,
	input_source_hand       = 1 << 0,
	input_source_hand_left  = 1 << 1,
	input_source_hand_right = 1 << 2,
	input_source_gaze       = 1 << 4,
	input_source_gaze_head  = 1 << 5,
	input_source_gaze_eyes  = 1 << 6,
	input_source_gaze_cursor= 1 << 7,
	input_source_can_press  = 1 << 8,
} input_source_;
SK_MakeFlag(input_source_);

typedef enum handed_ {
	handed_left  = 0,
	handed_right = 1,
	handed_max   = 2,
} handed_;

typedef enum button_state_ {
	button_state_inactive      = 0,
	button_state_active        = 1 << 0,
	button_state_just_inactive = 1 << 1,
	button_state_just_active   = 1 << 2,
	button_state_changed       = button_state_just_inactive | button_state_just_active,
} button_state_;
SK_MakeFlag(button_state_);

typedef struct pointer_t {
	input_source_ source;
	button_state_ tracked;
	button_state_ state;
	ray_t         ray;
	quat          orientation;
} pointer_t;

typedef struct hand_joint_t {
	vec3  position;
	quat  orientation;
	float radius;
} hand_joint_t;

typedef struct hand_t {
	hand_joint_t fingers[5][5];
	pose_t  wrist;
	pose_t  palm;
	handed_ handedness;
	button_state_ tracked_state;
	button_state_ pinch_state;
	button_state_ grip_state;
	float pinch_activation;
	float grip_activation;
} hand_t;

typedef struct mouse_t {
	bool32_t available;
	vec2     pos;
	vec2     pos_change;
	float    scroll;
	float    scroll_change;
} mouse_t;

// Based on VK codes
typedef enum key_ {
	key_mouse_left= 0x01, key_mouse_right = 0x02, key_mouse_center = 0x04,
	key_backspace = 0x08, key_tab       = 0x09,
	key_return    = 0x0D, key_shift     = 0x10,
	key_ctrl      = 0x11, key_alt       = 0x12,
	key_caps_lock = 0x14,
	key_esc       = 0x1B, key_space     = 0x20,
	key_end       = 0x23, key_home      = 0x24,
	key_left      = 0x25, key_right     = 0x27,
	key_up        = 0x26, key_down      = 0x28,
	key_printscreen=0x2A, key_insert    = 0x2D, key_del = 0x2E,
	key_0 = 0x30, key_1 = 0x31, key_2 = 0x32, key_3 = 0x33, key_4 = 0x34, key_5 = 0x35, key_6 = 0x36, key_7 = 0x37, key_8 = 0x38, key_9 = 0x39, 
	key_a = 0x41, key_b = 0x42, key_c = 0x43, key_d = 0x44, key_e = 0x45, key_f = 0x46, key_g = 0x47, key_h = 0x48, key_i = 0x49, key_j = 0x4A, key_k = 0x4B, key_l = 0x4C, key_m = 0x4D, key_n = 0x4E, key_o = 0x4F, key_p = 0x50, key_q = 0x51, key_r = 0x52, key_s = 0x53, key_t = 0x54, key_u = 0x55, key_v = 0x56, key_w = 0x57, key_x = 0x58, key_y = 0x59, key_z = 0x5A,
	key_lcmd = 0x5B, key_rcmd = 0x5C,
	key_num0 = 0x60, key_num1 = 0x61, key_num2 = 0x62, key_num3 = 0x63, key_num4 = 0x64, key_num5 = 0x65, key_num6 = 0x66, key_num7 = 0x67, key_num8 = 0x68, key_num9 = 0x69,
	key_multiply = 0x6A, key_add = 0x6B, key_subtract = 0x6D, key_decimal = 0x6E, key_divide = 0x6F,
	key_f1 = 0x70, key_f2 = 0x71, key_f3 = 0x72, key_f4 = 0x73, key_f5 = 0x74, key_f6 = 0x75, key_f7 = 0x76, key_f8 = 0x77, key_f9 = 0x78, key_f10 = 0x79, key_f11 = 0x7A, key_f12 = 0x7B,
	key_MAX = 0xFF,
} key_;

SK_API int                   input_pointer_count(input_source_ filter sk_default(input_source_any));
SK_API pointer_t             input_pointer      (int32_t index, input_source_ filter sk_default(input_source_any));
SK_API const sk_ref(hand_t)  input_hand         (handed_ hand);
SK_API void                  input_hand_override(handed_ hand, hand_joint_t *hand_joints);
SK_API const sk_ref(pose_t)  input_head         ();
SK_API const sk_ref(mouse_t) input_mouse        ();
SK_API button_state_         input_key          (key_ key);
SK_API void                  input_hand_visible (handed_ hand, bool32_t visible);
SK_API void                  input_hand_solid   (handed_ hand, bool32_t solid);
SK_API void                  input_hand_material(handed_ hand, material_t material);

SK_API void input_subscribe  (input_source_ source, button_state_ event, void (*event_callback)(input_source_ source, button_state_ event, const sk_ref(pointer_t) pointer));
SK_API void input_unsubscribe(input_source_ source, button_state_ event, void (*event_callback)(input_source_ source, button_state_ event, const sk_ref(pointer_t) pointer));
SK_API void input_fire_event (input_source_ source, button_state_ event, const sk_ref(pointer_t) pointer);

///////////////////////////////////////////

SK_API pose_t pose_from_spatial(uint8_t spatial_graph_node_id[16]);

///////////////////////////////////////////

typedef enum log_{
	log_diagnostic = 0,
	log_inform,
	log_warning,
	log_error
} log_;
typedef enum log_colors_ {
	log_colors_ansi = 0,
	log_colors_none
} log_colors_;

       void log_diag      (const char* text);
       void log_diagf     (const char* text, ...);
       void log_info      (const char* text);
       void log_infof     (const char* text, ...);
       void log_warn      (const char* text);
       void log_warnf     (const char* text, ...);
       void log_err       (const char* text);
       void log_errf      (const char* text, ...);
       void log_writef    (log_ level, const char *text, ...);
SK_API void log_write     (log_ level, const char* text);
SK_API void log_set_filter(log_ level);
SK_API void log_set_colors(log_colors_ colors);
SK_API void log_subscribe  (void (*on_log)(log_, const char*));
SK_API void log_unsubscribe(void (*on_log)(log_, const char*));

///////////////////////////////////////////

static const char *default_id_material             = "default/material";
static const char *default_id_material_equirect    = "default/equirect_convert";
static const char *default_id_material_font        = "default/material_font";
static const char *default_id_material_hand        = "default/material_hand";
static const char *default_id_material_ui          = "default/material_ui";
static const char *default_id_material_ui_quadrant = "default/material_ui_quadrant";
static const char *default_id_tex       = "default/tex";
static const char *default_id_tex_black = "default/tex_black";
static const char *default_id_tex_gray  = "default/tex_gray";
static const char *default_id_tex_flat  = "default/tex_flat";
static const char *default_id_tex_rough = "default/tex_rough";
static const char *default_id_cubemap   = "default/cubemap";
static const char *default_id_font      = "default/font";
static const char *default_id_mesh_quad      = "default/mesh_quad";
static const char *default_id_mesh_cube      = "default/mesh_cube";
static const char *default_id_mesh_sphere    = "default/mesh_sphere";
static const char *default_id_mesh_lefthand  = "default/mesh_lefthand";
static const char *default_id_mesh_righthand = "default/mesh_righthand";
static const char *default_id_mesh_ui_button = "default/mesh_ui_button";
static const char *default_id_shader             = "default/shader";
static const char *default_id_shader_pbr         = "default/shader_pbr";
static const char *default_id_shader_unlit       = "default/shader_unlit";
static const char *default_id_shader_font        = "default/shader_font";
static const char *default_id_shader_equirect    = "default/shader_equirect";
static const char *default_id_shader_ui          = "default/shader_ui";
static const char *default_id_shader_ui_quadrant = "default/shader_ui_quadrant";
static const char *default_id_sound_click   = "default/sound_click";
static const char *default_id_sound_unclick = "default/sound_unclick";
static const char *default_id_sound_grab    = "default/sound_grab";
static const char *default_id_sound_ungrab  = "default/sound_ungrab";

#ifdef __cplusplus
} // namespace sk
#endif

///////////////////////////////////////////
// Tools for creating the version information

#define SK_STR2(x) #x
#define SK_STR(x) SK_STR2(x)

  // This will look like 'M.i.P-rcr', or 'M.i.P' if r is 0
#if SK_VERSION_PRERELEASE != 0
#define SK_VERSION (SK_STR(SK_VERSION_MAJOR) "." SK_STR(SK_VERSION_MINOR) "." SK_STR(SK_VERSION_PATCH) "-rc" SK_STR(SK_VERSION_PRERELEASE))
#else
#define SK_VERSION (SK_STR(SK_VERSION_MAJOR) "." SK_STR(SK_VERSION_MINOR) "." SK_STR(SK_VERSION_PATCH))
#endif

  // A version in hex looks like: 0xMMMMiiiiPPPPrrrr
  // In order of Major.mInor.Patch.pre-Release
#define SK_VERSION_ID ( ((uint64_t)SK_VERSION_MAJOR << 48) | ((uint64_t)SK_VERSION_MINOR << 32) | ((uint64_t)SK_VERSION_PATCH << 16) | (uint64_t)(SK_VERSION_PRERELEASE) )
