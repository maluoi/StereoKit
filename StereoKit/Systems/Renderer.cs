
using System;

namespace StereoKit
{
	/// <summary>Do you need to draw something? Well, you're probably in the right place!
	/// This static class includes a variety of different drawing methods, from rendering 
	/// Models and Meshes, to setting rendering options and drawing to offscreen surfaces!
	/// Even better, it's entirely a static class, so you can call it from anywhere :)</summary>
	public class Renderer
	{
		/// <summary>Set a cubemap skybox texture for rendering a background! This is only visible on Opaque
		/// displays, since transparent displays have the real world behind them already! StereoKit has a
		/// a default procedurally generated skybox. You can load one with `Tex.FromEquirectangular`, 
		/// `Tex.GenCubemap`. If you're trying to affect the lighting, see `Renderer.SkyLight`.</summary>
		public static Tex SkyTex
		{
			get  { IntPtr ptr = NativeAPI.render_get_skytex(); return ptr == IntPtr.Zero ? null : new Tex(ptr); }
			set => NativeAPI.render_set_skytex(value._inst);
		}

		/// <summary>Sets the lighting information for the scene! You can build one through 
		/// `SphericalHarmonics.FromLights`, or grab one from `Tex.FromEquirectangular` or 
		/// `Tex.GenCubemap`</summary>
		public static SphericalHarmonics SkyLight
		{
			set => NativeAPI.render_set_skylight(value);
		}

		/// <summary>Enables or disables rendering of the skybox texture! It's enabled by default on Opaque
		/// displays, and completely unavailable for transparent displays.</summary>
		public static bool EnableSky
		{
			get => NativeAPI.render_enabled_skytex();
			set => NativeAPI.render_enable_skytex(value);
		}

		/// <summary>This is the color the renderer will clear the screen to
		/// when beginning to draw a new frame. This is ignored on displays 
		/// with transparent screens</summary>
		public static Color32 ClearColor
		{
			set => NativeAPI.render_set_clear_color(value);
		}

		/// <summary>Sets and gets the root transform of the camera! This
		/// will be the identity matrix by default. The user's head  location
		/// will then be relative to this point. This is great to use if 
		/// you're trying to do teleportation, redirected walking, or just 
		/// shifting the floor around.</summary>
		public static Matrix CameraRoot 
		{
			get => NativeAPI.render_get_cam_root();
			set => NativeAPI.render_set_cam_root(value);
		}

		public static Matrix Projection => NativeAPI.render_get_projection();

		/// <summary>Adds a mesh to the render queue for this frame! If the Hierarchy has a transform on it,
		/// that transform is combined with the Matrix provided here.</summary>
		/// <param name="mesh">A valid Mesh you wish to draw.</param>
		/// <param name="material">A Material to apply to the Mesh.</param>
		/// <param name="transform">A Matrix that will transform the mesh from Model Space into the current
		/// Hierarchy Space.</param>
		public static void Add(Mesh mesh, Material material, Matrix transform)
			=> NativeAPI.render_add_mesh(mesh._inst, material._inst, transform, Color.White);
		/// <summary>Adds a mesh to the render queue for this frame! If the Hierarchy has a transform on it,
		/// that transform is combined with the Matrix provided here.</summary>
		/// <param name="mesh">A valid Mesh you wish to draw.</param>
		/// <param name="material">A Material to apply to the Mesh.</param>
		/// <param name="transform">A Matrix that will transform the mesh from Model Space into the current
		/// Hierarchy Space.</param>
		/// <param name="color">A per-instance color value to pass into the shader! Normally this gets used 
		/// like a material tint. If you're adventurous and don't need per-instance colors, this is a great 
		/// spot to pack in extra per-instance data for the shader!</param>
		public static void Add(Mesh mesh, Material material, Matrix transform, Color color)
			=> NativeAPI.render_add_mesh(mesh._inst, material._inst, transform, color);

		/// <summary>Adds a Model to the render queue for this frame! If the Hierarchy has a transform on it,
		/// that transform is combined with the Matrix provided here.</summary>
		/// <param name="model">A valid Model you wish to draw.</param>
		/// <param name="transform">A Matrix that will transform the Model from Model Space into the current
		/// Hierarchy Space.</param>
		public static void Add(Model model, Matrix transform)
			=> NativeAPI.render_add_model(model._inst, transform, Color.White);
		/// <summary>Adds a Model to the render queue for this frame! If the Hierarchy has a transform on it,
		/// that transform is combined with the Matrix provided here.</summary>
		/// <param name="model">A valid Model you wish to draw.</param>
		/// <param name="transform">A Matrix that will transform the Model from Model Space into the current
		/// Hierarchy Space.</param>
		/// <param name="color">A per-instance color value to pass into the shader! Normally this gets used 
		/// like a material tint. If you're adventurous and don't need per-instance colors, this is a great 
		/// spot to pack in extra per-instance data for the shader!</param>
		public static void Add(Model model, Matrix transform, Color color)
			=> NativeAPI.render_add_model(model._inst, transform, color);

		/// <summary>Set the near and far clipping planes of the camera! These are important
		/// to z-buffer quality, especially when using low bit depth z-buffers as recommended
		/// for devices like the HoloLens. The smaller the range between the near and far planes,
		/// the better your z-buffer will look! If you see flickering on objects that are overlapping,
		/// try making the range smaller. </summary>
		/// <param name="nearPlane">The GPU discards pixels that are too close to the camera, this
		/// is that distance! It must be larger than zero, due to the projection math, which also means
		/// that numbers too close to zero may also produce weird results? This has an enforced minimum
		/// of 0.0001.</param>
		/// <param name="farPlane">At what distance from the camera does the GPU discard pixel? This is
		/// not true distance, but rather Z-axis distance from zero in View Space coordinates!</param>
		public static void SetClip(float nearPlane = 0.01f, float farPlane = 50)
			=> NativeAPI.render_set_clip(nearPlane, farPlane);

		/// <summary>Only works for flatscreen! This updates the camera's 
		/// projection matrix with a new field of view.</summary>
		/// <param name="fieldOfViewDegrees">Horizontal field of view in
		/// degrees.</param>
		public static void SetFOV(float fieldOfViewDegrees)
			=> NativeAPI.render_set_fov(fieldOfViewDegrees);

		/// <summary>Renders a Material onto a rendertarget texture! StereoKit uses a 4 vert quad stretched
		/// over the surface of the texture, and renders the material onto it to the texture.</summary>
		/// <param name="toRendertarget">A texture that's been set up as a render target!</param>
		/// <param name="material">This material is rendered onto the texture! Set it up like you would
		/// if you were applying it to a plane, or quad mesh.</param>
		public static void Blit(Tex toRendertarget, Material material)
			=> NativeAPI.render_blit(toRendertarget._inst, material._inst);

		/// <summary>Schedules a screenshot for the end of the frame! The view will be
		/// rendered from the given position at the given point, with a resolution the same
		/// size as the screen's surface. It'll be saved as a .jpg file at the filename
		/// provided.</summary>
		/// <param name="from">Viewpoint location.</param>
		/// <param name="at">Direction the viewpoint is looking at.</param>
		/// <param name="width">Size of the screenshot horizontally, in pixels.</param>
		/// <param name="height">Size of the screenshot vertically, in pixels.</param>
		/// <param name="filename">Filename to write the screenshot to! Note this'll be a 
		/// .jpg regardless of what file extension you use right now.</param>
		public static void Screenshot(Vec3 from, Vec3 at, int width, int height, string filename)
			=> NativeAPI.render_screenshot(from, at, width, height, filename);

	}
}