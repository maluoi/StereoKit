using System;
using System.Runtime.InteropServices;
using System.Text;

namespace StereoKit
{
	internal static class NativeAPI
	{
		const string            dll  = "StereoKitC.dll";
		const CharSet           cSet = CharSet.Ansi;
		const CallingConvention call = CallingConvention.Cdecl;

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int        sk_init(string app_name, Runtime preferred_runtime, int fallback = 1);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void       sk_shutdown();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void       sk_quit();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool       sk_step([MarshalAs(UnmanagedType.FunctionPtr)] Action app_update);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Runtime    sk_active_runtime();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void       sk_set_settings(Settings settings);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern SystemInfo sk_system_info();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr     sk_version_name();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern ulong      sk_version_id();

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern float   time_getf_unscaled();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern double  time_get_unscaled();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern float   time_getf();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern double  time_get();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern float   time_elapsedf_unscaled();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern double  time_elapsed_unscaled();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern float   time_elapsedf();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern double  time_elapsed();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void    time_scale(double scale);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void    time_set_time(double total_seconds, double frame_elapsed_seconds=0);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Quat quat_difference (in Quat a, in Quat b);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Quat quat_lookat     (in Vec3 from, in Vec3 at);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Quat quat_lookat_up  (in Vec3 from, in Vec3 at, in Vec3 up);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Quat quat_from_angles(float pitch_x_deg, float yaw_y_deg, float roll_z_deg);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Quat quat_slerp      (in Quat a, in Quat b, float t);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Quat quat_normalize  (in Quat a);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Quat quat_inverse    (in Quat a);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Quat quat_mul        (in Quat a, in Quat b);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Vec3 quat_mul_vec    (in Quat a, in Vec3 b);

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   matrix_inverse      (in Matrix a, out Matrix out_matrix);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   matrix_mul          (in Matrix a, in Matrix b, out Matrix out_matrix);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Vec3   matrix_mul_point    (in Matrix transform, in Vec3 point);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Vec3   matrix_mul_direction(in Matrix transform, in Vec3 direction);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Matrix matrix_trs          (in Vec3 position, in Quat orientation, in Vec3 scale);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   matrix_trs_out      (out Matrix out_result, in Vec3 position, in Quat orientation, in Vec3 scale);

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Matrix pose_matrix(in Pose pose, Vec3 scale);

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Vec3   vec3_cross(in Vec3 a, in Vec3 b);
        
		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool plane_ray_intersect  (Plane plane, Ray ray, out Vec3 out_pt);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool plane_line_intersect (Plane plane, Vec3 p1, Vec3 p2, out Vec3 out_pt);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Vec3 plane_point_closest  (Plane plane, Vec3 pt);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool sphere_ray_intersect (Sphere sphere, Ray ray, out Vec3 out_pt);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool sphere_point_contains(Sphere sphere, Vec3 pt);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool bounds_ray_intersect (Bounds bounds, Ray ray, out Vec3 out_pt);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool bounds_point_contains(Bounds bounds, Vec3 pt);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool bounds_line_contains (Bounds bounds, Vec3 linePt1, Vec3 linePt2);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Color color_hsv   (float hue, float saturation, float value, float transparency);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Vec3  color_to_hsv(in Color color);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Color color_lab   (float l, float a, float b, float transparency);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Vec3  color_to_lab(in Color color);

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr  gradient_create ();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr  gradient_create_keys([In] GradientKey[] keys, int count);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void    gradient_add    (IntPtr gradient, Color color, float position);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void    gradient_set    (IntPtr gradient, int index, Color color, float position);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void    gradient_remove (IntPtr gradient, int index);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int     gradient_count  (IntPtr gradient);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Color   gradient_get    (IntPtr gradient, float at);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Color32 gradient_get32  (IntPtr gradient, float at);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void    gradient_release(IntPtr gradient);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern SphericalHarmonics sh_create([In] SHLight[] lights, int light_count);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Color              sh_lookup(in SphericalHarmonics lookup, Vec3 normal);


		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr mesh_find         (string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr mesh_create       ();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   mesh_set_id       (IntPtr mesh, string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   mesh_release      (IntPtr mesh);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   mesh_set_keep_data(IntPtr mesh, bool keep_data);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool   mesh_get_keep_data(IntPtr mesh);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   mesh_set_verts    (IntPtr mesh, [In] Vertex[] vertices, int vertex_count, bool calculate_bounds = true);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   mesh_get_verts    (IntPtr mesh, out IntPtr out_vertices, out int out_vertex_count);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   mesh_set_inds     (IntPtr mesh, [In] uint[] indices, int index_count);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   mesh_get_inds     (IntPtr mesh, out IntPtr out_indices,  out int out_index_count); // [Out, MarshalAs(unmanagedType:UnmanagedType.LPArray, SizeParamIndex=2)]
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   mesh_set_draw_inds(IntPtr mesh, int index_count);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   mesh_set_bounds   (IntPtr mesh, in Bounds bounds);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Bounds mesh_get_bounds   (IntPtr mesh);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool   mesh_ray_intersect(IntPtr mesh, Ray model_space_ray, out Vec3 out_pt);

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr mesh_gen_plane       (Vec2 dimensions, Vec3 plane_normal, Vec3 plane_top_direction, int subdivisions = 0);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr mesh_gen_cube        (Vec3 dimensions, int subdivisions = 0);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr mesh_gen_sphere      (float diameter, int subdivisions = 4);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr mesh_gen_rounded_cube(Vec3 dimensions, float edge_radius, int subdivisions);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr mesh_gen_cylinder    (float diameter, float depth, Vec3 direction, int subdivisions);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr tex_find                (string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr tex_create              (TexType type = TexType.Image, TexFormat format = TexFormat.Rgba32);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr tex_create_mem          ([In] byte[] data, int data_size, bool srgb_data);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr tex_create_file         (string file, bool srgb_data);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr tex_create_cubemap_file (string equirectangular_file,    bool srgb_data, IntPtr lighting_info);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr tex_create_cubemap_file (string equirectangular_file,    bool srgb_data, out SphericalHarmonics lighting_info);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr tex_create_cubemap_files(string[] cube_face_file_xxyyzz, bool srgb_data, IntPtr lighting_info);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr tex_create_cubemap_files(string[] cube_face_file_xxyyzz, bool srgb_data, out SphericalHarmonics lighting_info);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   tex_set_id              (IntPtr texture, string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   tex_release             (IntPtr texture);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   tex_set_colors          (IntPtr texture, int width, int height, IntPtr data);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   tex_set_colors          (IntPtr texture, int width, int height, [In] Color32[] data);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   tex_set_colors          (IntPtr texture, int width, int height, [In] Color[] data);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   tex_set_colors          (IntPtr texture, int width, int height, [In] byte[] data);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   tex_set_colors          (IntPtr texture, int width, int height, [In] ushort[] data);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   tex_set_colors          (IntPtr texture, int width, int height, [In] float[] data);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr tex_add_zbuffer         (IntPtr texture, TexFormat format = TexFormat.DepthStencil);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   tex_rtarget_clear       (IntPtr render_target, Color32 color);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   tex_rtarget_set_active  (IntPtr render_target);
		//[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   tex_get_data            (IntPtr texture, void* out_data, ulong out_data_size);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr tex_gen_cubemap         (IntPtr gradient, Vec3 gradient_dir, int resolution, IntPtr lighting_info);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr tex_gen_cubemap         (IntPtr gradient, Vec3 gradient_dir, int resolution, out SphericalHarmonics lighting_info);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr tex_gen_cubemap_sh      (in SphericalHarmonics lighting, int resolution);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern TexFormat  tex_get_format      (IntPtr texture);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int        tex_get_width       (IntPtr texture);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int        tex_get_height      (IntPtr texture);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void       tex_set_sample      (IntPtr texture, TexSample sample = TexSample.Linear);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern TexSample  tex_get_sample      (IntPtr texture);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void       tex_set_address     (IntPtr texture, TexAddress address_mode = TexAddress.Wrap);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern TexAddress tex_get_address     (IntPtr texture);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void       tex_set_anisotropy  (IntPtr texture, int anisotropy_level = 4);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int        tex_get_anisotropy  (IntPtr texture);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr font_find   (string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr font_create (string file);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   font_release(IntPtr font);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr font_get_tex(IntPtr font);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr shader_find        (string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool   shader_compile     (string hlsl, string opt_filename, out IntPtr out_data, out ulong out_size);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool   shader_compile     (string hlsl, IntPtr opt_filename, out IntPtr out_data, out ulong out_size);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr shader_create_file (string filename);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr shader_create_hlsl (string hlsl);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr shader_create_mem  ([In] byte[] data, ulong data_size);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   shader_set_id      (IntPtr shader, string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern string shader_get_name    (IntPtr shader);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int    shader_set_code    (IntPtr shader, [In] byte[] data, int data_size);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int    shader_set_codefile(IntPtr shader, string filename);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   shader_release     (IntPtr shader);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr material_find            (string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr material_create          (IntPtr shader);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr material_copy            (IntPtr material);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr material_copy_id         (string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   material_set_id          (IntPtr material, string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   material_release         (IntPtr material);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   material_set_transparency(IntPtr material, Transparency mode);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   material_set_cull        (IntPtr material, Cull  mode);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   material_set_wireframe   (IntPtr material, bool wireframe);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   material_set_queue_offset(IntPtr material, int offset);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   material_set_float       (IntPtr material, string name, float   value);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   material_set_color       (IntPtr material, string name, Color   value);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   material_set_vector      (IntPtr material, string name, Vec4    value);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   material_set_matrix      (IntPtr material, string name, Matrix  value);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int    material_set_texture     (IntPtr material, string name, IntPtr  value);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int    material_set_texture_id  (IntPtr material, ulong  id,   IntPtr  value);
		//[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   material_set_param       (IntPtr material, string name, MaterialParam type, const void *value);
		//[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   material_set_param_id    (IntPtr material, ulong    id,   MaterialParam type, const void *value);
		//[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int    material_get_param       (IntPtr material, string name, MaterialParam type, void *out_value);
		//[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int    material_get_param_id    (IntPtr material, ulong    id, MaterialParam type, void *out_value);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   material_get_param_info  (IntPtr material, int index, out string out_name, out MaterialParam out_type);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int    material_get_param_count (IntPtr material);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   material_set_shader      (IntPtr material, IntPtr shader);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr material_get_shader      (IntPtr material);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern TextStyle text_make_style(IntPtr font, float character_height, IntPtr material, Color32 color);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void      text_add_at    (string text, in Matrix transform, int style, TextAlign position = TextAlign.Center, TextAlign align = TextAlign.Center, float off_x = 0, float off_y = 0, float off_z = 0);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void      text_add_in    (string text, in Matrix transform, Vec2 size, TextFit fit, int style, TextAlign position = TextAlign.Center, TextAlign align = TextAlign.Center, float off_x = 0, float off_y = 0, float off_z = 0);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Vec2      text_size      (string text, int style);


		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr solid_create          (ref Vec3 position, ref Quat rotation, SolidType type = SolidType.Normal);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   solid_release         (IntPtr solid);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   solid_add_sphere      (IntPtr solid, float diameter,               float kilograms, in Vec3 offset);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   solid_add_box         (IntPtr solid, in Vec3 dimensions,           float kilograms, in Vec3 offset);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   solid_add_capsule     (IntPtr solid, float diameter, float height, float kilograms, in Vec3 offset);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   solid_set_type        (IntPtr solid, SolidType type);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   solid_set_enabled     (IntPtr solid, int enabled);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   solid_move            (IntPtr solid, in Vec3 position, in Quat rotation);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   solid_teleport        (IntPtr solid, in Vec3 position, in Quat rotation);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   solid_set_velocity    (IntPtr solid, in Vec3 meters_per_second);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   solid_set_velocity_ang(IntPtr solid, in Vec3 radians_per_second);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   solid_get_pose        (IntPtr solid, out Pose out_pose);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr model_find         (string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr model_create       ();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr model_create_mesh  (IntPtr mesh, IntPtr material);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr model_create_mem   (string filename, [In] byte[] data, int data_size, IntPtr shader);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr model_create_file  (string filename, IntPtr shader);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   model_set_id       (IntPtr model, string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   model_release      (IntPtr model);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr model_get_material (IntPtr model, int subset);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr model_get_mesh     (IntPtr model, int subset);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Matrix model_get_transform(IntPtr model, int subset);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   model_set_material (IntPtr model, int subset, IntPtr material);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   model_set_mesh     (IntPtr model, int subset, IntPtr mesh);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   model_set_transform(IntPtr model, int subset, in Matrix transform);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   model_remove_subset(IntPtr model, int subset);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int    model_add_subset   (IntPtr model, IntPtr mesh, IntPtr material, in Matrix transform);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int    model_subset_count (IntPtr model);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   model_recalculate_bounds(IntPtr model);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   model_set_bounds   (IntPtr model, in Bounds bounds);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Bounds model_get_bounds   (IntPtr model);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr sprite_create     (IntPtr sprite,   SpriteType type = SpriteType.Atlased, string atlas_id = "default");
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr sprite_create_file(string filename, SpriteType type = SpriteType.Atlased, string atlas_id = "default");
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   sprite_set_id     (IntPtr sprite, string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   sprite_release    (IntPtr sprite);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern float  sprite_get_aspect (IntPtr sprite);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   sprite_draw       (IntPtr sprite, in Matrix transform, Color32 color);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int    sprite_get_width  (IntPtr sprite);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int    sprite_get_height (IntPtr sprite);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Vec2   sprite_get_dimensions_normalized(IntPtr sprite);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void line_add(Vec3 start, Vec3 end, Color32 color_start, Color32 color_end, float thickness);
		//[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void line_addv(line_point_t start, line_point_t end);
		//[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void line_add_list(const Vec3* points, int count, Color32 color, float thickness);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void line_add_listv([In] LinePoint[] points, int count);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   render_set_clip      (float near_plane, float far_plane);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   render_set_fov       (float field_of_view_degrees);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Matrix render_get_projection();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Matrix render_get_cam_root  ();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   render_set_cam_root  (Matrix cam_root);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   render_set_skytex    (IntPtr sky_texture);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr render_get_skytex    ();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   render_set_skylight  (in SphericalHarmonics lighting_info);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   render_set_clear_color(Color32 color);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   render_enable_skytex (bool show_sky);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool   render_enabled_skytex();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   render_add_mesh      (IntPtr mesh, IntPtr material, in Matrix transform, Color color);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   render_add_model     (IntPtr model, in Matrix transform, Color color);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   render_blit          (IntPtr to_rendertarget, IntPtr material);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   render_screenshot    (Vec3 from_viewpt, Vec3 at, int width, int height, string file);
		//[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void render_get_device  (void **device, void **context);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void hierarchy_push(in Matrix transform);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void hierarchy_pop();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void hierarchy_set_enabled(bool enabled);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool hierarchy_is_enabled();
		//[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern ref Matrix hierarchy_to_world();
		//[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern ref Matrix hierarchy_to_local();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Vec3 hierarchy_to_local_point    (in Vec3 world_pt);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Vec3 hierarchy_to_local_direction(in Vec3 world_dir);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Quat hierarchy_to_local_rotation (in Quat world_orientation);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Vec3 hierarchy_to_world_point    (in Vec3 local_pt);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Vec3 hierarchy_to_world_direction(in Vec3 local_dir);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Quat hierarchy_to_world_rotation (in Quat local_orientation);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr sound_find    (string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   sound_set_id  (IntPtr sound, string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr sound_create  (string filename);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr sound_generate([MarshalAs(UnmanagedType.FunctionPtr)] AudioGenerator function, float duration);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   sound_play    (IntPtr sound, Vec3 at, float volume);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void   sound_release (IntPtr sound);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern int     input_pointer_count(InputSource filter = InputSource.Any);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Pointer input_pointer      (int index, InputSource filter = InputSource.Any);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr  input_hand         (Handed hand);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void    input_hand_override(Handed hand, [In] HandJoint[] hand_joints);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void    input_hand_override(Handed hand, IntPtr hand_joints);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr  input_mouse        ();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern IntPtr  input_head         ();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern BtnState input_key         (Key key);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void    input_hand_visible (Handed hand, bool visible);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void    input_hand_solid   (Handed hand, bool solid);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void    input_hand_material(Handed hand, IntPtr material);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Pose pose_from_spatial([MarshalAs(UnmanagedType.LPArray, SizeConst = 16)] byte[] spatial_graph_node_id);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void input_subscribe  (InputSource source, BtnState evt, InputEventCallback event_callback);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void input_unsubscribe(InputSource source, BtnState evt, InputEventCallback event_callback);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void input_fire_event (InputSource source, BtnState evt, IntPtr pointer);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void log_write      (LogLevel level, string text);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void log_set_filter (LogLevel level);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void log_subscribe  (LogCallback on_log);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void log_unsubscribe(LogCallback on_log);

		///////////////////////////////////////////

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void ui_show_volumes(bool show);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void ui_settings    (UISettings settings);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void ui_set_color   (Color color);

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void ui_layout_area (Vec3 start, Vec2 dimensions);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern Vec2 ui_area_remaining();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void ui_nextline    ();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void ui_sameline    ();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern float ui_line_height();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void ui_reserve_box (Vec2 size);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void ui_space       (float space);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern ulong ui_push_id(string id);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void ui_pop_id();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool ui_is_interacting(Handed hand);

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool ui_volume_at      (string id, Bounds bounds);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern BtnState ui_interact_volume_at(Bounds bounds, out Handed out_hand);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool ui_button_at      (string text, Vec3 window_relative_pos, Vec2 size);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool ui_button_round_at(string text, IntPtr image_sprite, Vec3 window_relative_pos, float diameter);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool ui_toggle_at      (string text, ref bool pressed, Vec3 window_relative_pos, Vec2 size);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool ui_hslider_at     (string id,   ref float value, float min, float max, float step, Vec3 window_relative_pos, Vec2 size);

		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void ui_label       (string text, bool use_padding);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void ui_label_sz    (string text, Vec2 size);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void ui_image       (IntPtr sprite_image, Vec2 size);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool ui_button      (string text);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool ui_button_sz   (string text, Vec2 size);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool ui_button_round(string id, IntPtr image, float diameter);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool ui_toggle      (string text, ref bool pressed);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool ui_toggle_sz   (string text, ref bool pressed, Vec2 size);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool ui_input       (string id,   StringBuilder buffer, int buffer_size, Vec2 size);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool ui_hslider     (string id,   ref float value, float min, float max, float step, float width = 0);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern bool ui_handle_begin(string text, ref Pose movement, Bounds handle, bool draw, UIMove move_type);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void ui_handle_end  ();
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void ui_window_begin(string text, ref Pose pose, Vec2 size, UIWin window_type, UIMove move_type);
		[DllImport(dll, CharSet = cSet, CallingConvention = call)] public static extern void ui_window_end  ();
	}
}
