rollout MinSwp_PM_Exporter_roll "MinSwp_Poly_Mesh_Exporter"
(
	local MINSWP_PM_FILE_ID = #(0x0F24606B1BAB4E67L, 0xB8E1906CD3053E02L );
	--local VersionOf_MW3D_PM = 139511231820L;
	local MINSWP_PM_VERSION   = 139511271405L; -- face_ID added
	
	function If_Esc_Then_YesNoMsg =
	(
		if keyboard.escPressed then
			return (queryBox "Are you sure you want to exit export ?" title:"MinSwp_PolyMesh" beep:true);
		else
			return false;
	);	
	function SceneExport =
	(
		local filename_ = undefined;
		filename_ = getsavefilename caption:"Minesweeper3D Poly Mesh" types:"MineSweeper Poly Mesh (*.MSPM)|*.MSPM|All Files (*.*)|*.*|"
		if (filename_ == undefined) then
		(
			return false;
		);
		max create mode;
		MinSwp_PM_Exporter_roll.prgbr.visible = true;
		local f = fopen filename_ "wb";
		-------------------------------
		writelonglong f (MINSWP_PM_FILE_ID[1]);
		writelonglong f (MINSWP_PM_FILE_ID[2]);
		writelonglong f MINSWP_PM_VERSION;
		Obj = selection[1];
		if (superclassof Obj != GeometryClass) then
		(
			messagebox "please select one geometry object";
			fclose f;
			return false;
		);
		---------------------------------------	
		local obj_faces_count = Obj.faces.count;
		local obj_verts_count = Obj.verts.count;
		writelong f (obj_verts_count as Integer);
		writelong f (obj_faces_count as Integer);
		MinSwp_PM_Exporter_roll.prgbr.value = 0;
		for i = 1 to obj_verts_count do
		(
			if (If_Esc_Then_YesNoMsg  ()) then
			(
				fclose f;
				return false;
			);
			local xyz = Obj.verts[i].pos;
			writefloat f (xyz.x as float);
			writefloat f (xyz.y as float);
			writefloat f (xyz.z as float);
			
			MinSwp_PM_Exporter_roll.prgbr.value = (100.0 * i) / obj_verts_count;
		);
		for i = 1 to obj_faces_count do
		(
			if (If_Esc_Then_YesNoMsg  ()) then
			(
				fclose f;
				return false;
			);
			local verts_indices = polyop.getFaceVerts Obj i;
			local verts_indices_count = verts_indices.count;
			writelong f (verts_indices_count as Integer);
			for j = 1 to verts_indices_count do 
			(
				writelong f ((verts_indices[j] - 1) as Integer);
			);
			
			/*
			xyz_indxs = getface obj_mesh i;
			uvw_indxs = meshop.getMapFace obj_mesh 1 i;
			for j = 1 to 3 do
			(
				xyz = obj_mesh.verts[xyz_indxs[j]].pos;
				uvw = meshop.getMapVert obj_mesh 1 (uvw_indxs[j]);
				-----------------------------------------
				norm_indx = Obj.Edit_Normals.GetNormalID i j;
				norm = normalize (Obj.Edit_Normals.GetNormal norm_indx);
				-----------------------------------------
				writefloat f ( xyz.x   as float);
				writefloat f ( xyz.y   as float);
				writefloat f ( xyz.z   as float);
				
				writefloat f ( norm.x  as float);
				writefloat f ( norm.y  as float);
				writefloat f ( norm.z  as float);
				
				writefloat f ( uvw.x   as float);
				writefloat f ( 1.0 - uvw.y   as float);
			);
			*/
			MinSwp_PM_Exporter_roll.prgbr.value = (100.0 * i) / obj_faces_count;
		);
		local Obj_Mesh = obj.mesh;
		for i = 1 to obj_faces_count do
		(
			if (If_Esc_Then_YesNoMsg  ()) then
			(
				fclose f;
				return false;
			);
			local face_ID = getFaceMatID Obj_Mesh i;
			writelong f ((face_ID - 1) as Integer);
			MinSwp_PM_Exporter_roll.prgbr.value = (100.0 * i) / obj_faces_count;
		);
		
		----------------------------------------
		MinSwp_PM_Exporter_roll.prgbr.value = 0;
		fclose f;
		MinSwp_PM_Exporter_roll.prgbr.visible = false;
		return true;
	);
	
	progressbar prgbr "" visible:false;
	button btn "Export";
	on btn pressed do
	(
		SceneExport ();
		print "MineSweeper:Ok";
	);
);
createdialog MinSwp_PM_Exporter_roll;
