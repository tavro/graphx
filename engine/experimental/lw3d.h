#ifdef ENGINE_ENABLE_EXPERIMENTAL
	void LW3D_View(const std::array<float, 16>& m);
	void LW3D_World(const std::array<float, 16>& m);
	void LW3D_Projection(const std::array<float, 16>& m);
		
	void LW3D_DrawTriangles(engine::Decal* decal, const std::vector<std::array<float,3>>& pos, const std::vector<engine::float_vector_2d>& tex, const std::vector<engine::Pixel>& col);

	void LW3D_ModelTranslate(const float x, const float y, const float z);
		
	void LW3D_SetCameraAtTarget(const float fEyeX, const float fEyeY, const float fEyeZ,
		const float fTargetX, const float fTargetY, const float fTargetZ,
		const float fUpX = 0.0f, const float fUpY = 1.0f, const float fUpZ = 0.0f);
	void LW3D_SetCameraAlongDirection(const float fEyeX, const float fEyeY, const float fEyeZ,
		const float fDirX, const float fDirY, const float fDirZ,
		const float fUpX = 0.0f, const float fUpY = 1.0f, const float fUpZ = 0.0f);

	void LW3D_EnableDepthTest(const bool bEnableDepth);
	void LW3D_EnableBackfaceCulling(const bool bEnableCull);
#endif