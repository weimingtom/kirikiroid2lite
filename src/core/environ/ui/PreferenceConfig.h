extern void TVPInitTextureFormatList();
extern bool TVPIsSupportTextureFormat(GLenum fmt);

namespace {
static tPreferenceScreen RootPreference;
static tPreferenceScreen OpenglOptPreference, SoftRendererOptPreference;
static Size PrefListSize;

class tTVPPreferenceInfoConstant : public iTVPPreferenceInfo {
public:
	tTVPPreferenceInfoConstant(const std::string &cap) : iTVPPreferenceInfo(cap, "") {}
	virtual iPreferenceItem *createItem(int idx) override {
		LocaleConfigManager *locmgr = LocaleConfigManager::GetInstance();
		return CreatePreferenceItem<tPreferenceItemConstant>(idx, PrefListSize, locmgr->GetText(Caption));
	}
};

class tTVPPreferenceInfoCheckBox : public tTVPPreferenceInfo<bool> {
public:
	tTVPPreferenceInfoCheckBox(const std::string &cap, const std::string &key, bool defval)
		: tTVPPreferenceInfo<bool>(cap, key, defval) {}
	virtual iPreferenceItem *createItem(int idx) override {
		LocaleConfigManager *locmgr = LocaleConfigManager::GetInstance();
		return CreatePreferenceItem<tPreferenceItemCheckBox>(idx, PrefListSize, locmgr->GetText(Caption),
			[this](tPreferenceItemCheckBox* item) {
			item->_getter = std::bind(&PreferenceGetValueBool, Key, DefaultValue);
			item->_setter = std::bind(&PreferenceSetValueBool, Key, std::placeholders::_1);
		});
	}
};

class tTVPPreferenceInfoSelectList : public tTVPPreferenceInfo<std::string>, tPreferenceItemSelectListInfo {
public:
	tTVPPreferenceInfoSelectList(const std::string &cap, const std::string &key, const std::string &defval,
		const std::vector/*initializer_list*/<std::pair<std::string, std::string> > &listinfo)
		: tTVPPreferenceInfo<std::string>(cap, key, defval), ListInfo(listinfo){}
	virtual iPreferenceItem *createItem(int idx) override {
		LocaleConfigManager *locmgr = LocaleConfigManager::GetInstance();
		return CreatePreferenceItem<tPreferenceItemSelectList>(idx, PrefListSize, locmgr->GetText(Caption),
			[this](tPreferenceItemSelectList* item) {
			item->initInfo(this);
			item->_getter = std::bind(&PreferenceGetValueString, Key, DefaultValue);
			item->_setter = [/*this*/=](std::string v){ onSetValue(v); };
		});
	}
	virtual const std::vector<std::pair<std::string, std::string> >& getListInfo() override {
		return ListInfo;
	}
	virtual void onSetValue(const std::string &v) {
		PreferenceSetValueString(Key, v);
	}
	std::vector<std::pair<std::string, std::string> > ListInfo;
};

class my_tuple {
public:
	std::string first;
	std::string second;
	unsigned int third;

	my_tuple(std::string first_,
		std::string second_,
		unsigned int third_) : first(first_)
		,second(second_)
		,third(third_){ }
};

class tTVPPreferenceInfoTextureCompressionSelectList : public tTVPPreferenceInfoSelectList {
public:
	tTVPPreferenceInfoTextureCompressionSelectList(const std::string &cap, const std::string &key, const std::string &defval,
		//std::tuple<std::string, std::string, unsigned int>
		const std::vector/*initializer_list*/<my_tuple> &listinfo)
	: tTVPPreferenceInfoSelectList(cap, key, defval, std::vector/*initializer_list*/<std::pair<std::string, std::string> >())
	, TextFmtList(listinfo), ListInited(false)
	{}

	virtual const std::vector<std::pair<std::string, std::string> >& getListInfo() override {
		if (!ListInited) {
			ListInited = true;
			TVPInitTextureFormatList();
			//for (const auto &it : TextFmtList) {
			for (auto p_it = TextFmtList.begin(); p_it != TextFmtList.end(); ++p_it)
			{
				const auto& it = *p_it;
				unsigned int fmt = it.third;//std::get<2>(it);
				if (!fmt || TVPIsSupportTextureFormat(fmt)) {
					ListInfo.push_back(std::pair<std::string, std::string>(it.first, it.second));//(std::get<0>(it), std::get<1>(it)));
				}
			}
		}
		return ListInfo;
	}

	bool ListInited/* = false*/;
	//std::vector<std::tuple<std::string, std::string, unsigned int> > TextFmtList;
	std::vector<my_tuple> TextFmtList;
};

// class tTVPPreferenceInfoSelectRenderer : public tTVPPreferenceInfoSelectList {
// 	typedef tTVPPreferenceInfoSelectList inherit;
// public:
// 	tTVPPreferenceInfoSelectRenderer(const std::string &cap, const std::string &key, const std::string &defval,
// 		const std::initializer_list<std::pair<std::string, std::string> > &listinfo) : inherit(cap, key, defval, listinfo) {}
// 	virtual void onSetValue(const std::string &v) {
// 		inherit::onSetValue(v);
// 		if (v == "opengl") {
// 			TVPOnOpenGLRendererSelected(true);
// 		}
// 	}
// };

class tTVPPreferenceInfoSelectFile : public tTVPPreferenceInfo<std::string> {
public:
	tTVPPreferenceInfoSelectFile(const std::string &cap, const std::string &key, const std::string &defval)
		: tTVPPreferenceInfo<std::string>(cap, key, defval) {}

	virtual iPreferenceItem *createItem(int idx) override {
		LocaleConfigManager *locmgr = LocaleConfigManager::GetInstance();
		return CreatePreferenceItem<tPreferenceItemFileSelect>(idx, PrefListSize, locmgr->GetText(Caption),
			[this](tPreferenceItemFileSelect* item) {
			item->_getter = std::bind(&PreferenceGetValueString, Key, DefaultValue);
			item->_setter = std::bind(&PreferenceSetValueString, Key, std::placeholders::_1);
		});
	}
};

class tTVPPreferenceInfoRendererSubPref : public iTVPPreferenceInfo {
public:
	tTVPPreferenceInfoRendererSubPref(const std::string &cap) { Caption = cap; } // Key is useless
	static tPreferenceScreen* GetSubPreferenceInfo() {
		std::string renderer = PreferenceGetValueString("renderer", "software");
		if (renderer == "opengl")
			return &OpenglOptPreference;
		else if (renderer == "software")
			return &SoftRendererOptPreference;
		return nullptr;
	}
	virtual iPreferenceItem *createItem(int idx) override {
		LocaleConfigManager *locmgr = LocaleConfigManager::GetInstance();
		iPreferenceItem *ret = CreatePreferenceItem<tPreferenceItemSubDir>(idx, PrefListSize, locmgr->GetText(Caption));
		ret->addClickEventListener([](Ref*) {
			TVPMainScene::GetInstance()->pushUIForm(TVPGlobalPreferenceForm::create(GetSubPreferenceInfo()));
		});
		return ret;
	}
	virtual tPreferenceScreen* GetSubScreenInfo() { return GetSubPreferenceInfo(); }
};

class tTVPPreferenceInfoSubPref : public iTVPPreferenceInfo {
	tPreferenceScreen Preference;
public:
	tTVPPreferenceInfoSubPref(const std::string &title, const std::vector/*initializer_list*/<iTVPPreferenceInfo*> &elem)
		: Preference(title, elem)
	{
		Caption = title;
	}
	virtual iPreferenceItem *createItem(int idx) override {
		LocaleConfigManager *locmgr = LocaleConfigManager::GetInstance();
		iPreferenceItem *ret = CreatePreferenceItem<tPreferenceItemSubDir>(idx, PrefListSize, locmgr->GetText(Caption));
		ret->addClickEventListener([this](Ref*){
			TVPMainScene::GetInstance()->pushUIForm(TVPGlobalPreferenceForm::create(&Preference));
		});
		return ret;
	}
	virtual tPreferenceScreen* GetSubScreenInfo() { return &Preference; }
};

class tTVPPreferenceInfoSliderIcon : public tTVPPreferenceInfo<float> {
public:
	tTVPPreferenceInfoSliderIcon(const std::string &cap, const std::string &key, float defval)
		: tTVPPreferenceInfo<float>(cap, key, defval) {}
	virtual iPreferenceItem *createItem(int idx) override {
		LocaleConfigManager *locmgr = LocaleConfigManager::GetInstance();

		tPreferenceItemCursorSlider * ret = new tPreferenceItemCursorSlider(DefaultValue, TVPMainScene::convertCursorScale);
		ret->autorelease();
		ret->_getter = std::bind(&PreferenceGetValueFloat, Key, DefaultValue);
		ret->_setter = std::bind(&PreferenceSetValueFloat, Key, std::placeholders::_1);
		ret->initFromInfo(idx, PrefListSize, locmgr->GetText(Caption));
		return ret;
	}
};

class tTVPPreferenceInfoSliderText : public tTVPPreferenceInfo<float> {
public:
	tTVPPreferenceInfoSliderText(const std::string &cap, const std::string &key, float defval)
		: tTVPPreferenceInfo<float>(cap, key, defval) {}

	static std::string convertPercentScale(float scale) {
		char buf[16];
		sprintf(buf, "%d%%", (int)(scale * 100));
		return buf;
	}

	virtual iPreferenceItem *createItem(int idx) override {
		LocaleConfigManager *locmgr = LocaleConfigManager::GetInstance();
		tPreferenceItemTextSlider * ret = new tPreferenceItemTextSlider(DefaultValue, convertPercentScale);
		ret->autorelease();
		ret->_getter = std::bind(&PreferenceGetValueFloat, Key, DefaultValue);
		ret->_setter = std::bind(&PreferenceSetValueFloat, Key, std::placeholders::_1);
		ret->initFromInfo(idx, PrefListSize, locmgr->GetText(Caption));
		return ret;
	}
};

class tTVPPreferenceInfoFetchSDCardPermission : public iTVPPreferenceInfo {
public:
	tTVPPreferenceInfoFetchSDCardPermission(const std::string &cap) : iTVPPreferenceInfo(cap, "") {}
	virtual iPreferenceItem *createItem(int idx) override {
		LocaleConfigManager *locmgr = LocaleConfigManager::GetInstance();
		tPreferenceItemConstant* ret = CreatePreferenceItem<tPreferenceItemConstant>(idx, PrefListSize, locmgr->GetText(Caption));
		ret->setTouchEnabled(true);
		ret->addClickEventListener([](Ref*) {
			TVPFetchSDCardPermission();
		});
		return ret;
	}
};

static void initAllConfig() {
	if (!RootPreference.Preferences.empty()) return;
	RootPreference.Title = "preference_title";
	RootPreference.Preferences = std::vector<iTVPPreferenceInfo*>();
	RootPreference.Preferences.push_back(new tTVPPreferenceInfoCheckBox("preference_output_log", "outputlog", true));
	RootPreference.Preferences.push_back(new tTVPPreferenceInfoCheckBox("preference_show_fps", "showfps", false));


	std::vector/*initializer_list*/<std::pair<std::string, std::string> > listinfo;
	listinfo.push_back(std::pair<std::string, std::string>("preference_opengl", "opengl"));
	listinfo.push_back(std::pair<std::string, std::string>("preference_software", "software"));
	RootPreference.Preferences.push_back(new tTVPPreferenceInfoSelectList("preference_select_renderer", "renderer", "software", listinfo));
	RootPreference.Preferences.push_back(new tTVPPreferenceInfoRendererSubPref("preference_renderer_opt"));
	RootPreference.Preferences.push_back(new tTVPPreferenceInfoSelectFile("preference_default_font", "default_font", ""));
	RootPreference.Preferences.push_back(new tTVPPreferenceInfoCheckBox("preference_force_def_font", "force_default_font", false));

	std::vector/*initializer_list*/<std::pair<std::string, std::string> > listinfo44;
	listinfo44.push_back(std::pair<std::string, std::string>("preference_mem_unlimited", "unlimited"));
	listinfo44.push_back(std::pair<std::string, std::string>("preference_mem_high", "high"));
	listinfo44.push_back(std::pair<std::string, std::string>("preference_mem_medium", "medium"));
	listinfo44.push_back(std::pair<std::string, std::string>("preference_mem_low", "low"));

#ifdef CC_TARGET_OS_IPHONE
	RootPreference.Preferences.push_back(new tTVPPreferenceInfoSelectList("preference_mem_limit", "memusage", "high", listinfo44));
#else
	RootPreference.Preferences.push_back(new tTVPPreferenceInfoSelectList("preference_mem_limit", "memusage", "unlimited", listinfo44));
#endif

	RootPreference.Preferences.push_back(new tTVPPreferenceInfoCheckBox("preference_keep_screen_alive", "keep_screen_alive", true));
	RootPreference.Preferences.push_back(new tTVPPreferenceInfoSliderIcon("preference_virtual_cursor_scale", "vcursor_scale", 0.5f));
	RootPreference.Preferences.push_back(new tTVPPreferenceInfoSliderText("preference_menu_handler_opacity", "menu_handler_opa", 0.15f));
#ifdef GLOBAL_PREFERENCE
	RootPreference.Preferences.push_back(new tTVPPreferenceInfoCheckBox("preference_remember_last_path", "remember_last_path", true));
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	RootPreference.Preferences.push_back(new tTVPPreferenceInfoCheckBox("preference_hide_android_sys_btn", "hide_android_sys_btn", false));
	RootPreference.Preferences.push_back(new tTVPPreferenceInfoFetchSDCardPermission("preference_android_fetch_sdcard_permission"));
#endif
#endif
#if 0
	RootPreference.Preferences.push_back(new tTVPPreferenceInfo(tTVPPreferenceInfo::eTypeSubPref, "preference_custom_option"));
#endif

	SoftRendererOptPreference.Title = "preference_soft_renderer_opt";
	SoftRendererOptPreference.Preferences = std::vector<iTVPPreferenceInfo*>();

	std::vector/*initializer_list*/<std::pair<std::string, std::string> > listinfo23;
	listinfo23.push_back(std::pair<std::string, std::string>("preference_draw_thread_auto", "0"));
	listinfo23.push_back(std::pair<std::string, std::string>("preference_draw_thread_1", "1"));
	listinfo23.push_back(std::pair<std::string, std::string>("preference_draw_thread_2", "2"));
	listinfo23.push_back(std::pair<std::string, std::string>("preference_draw_thread_3", "3"));
	listinfo23.push_back(std::pair<std::string, std::string>("preference_draw_thread_4", "4"));
	listinfo23.push_back(std::pair<std::string, std::string>("preference_draw_thread_5", "5"));
	listinfo23.push_back(std::pair<std::string, std::string>("preference_draw_thread_6", "6"));
	listinfo23.push_back(std::pair<std::string, std::string>("preference_draw_thread_7", "7"));
	listinfo23.push_back(std::pair<std::string, std::string>("preference_draw_thread_8", "8"));
	SoftRendererOptPreference.Preferences.push_back(new tTVPPreferenceInfoSelectList("preference_multi_draw_thread", "software_draw_thread", "0", listinfo23));

	std::vector/*initializer_list*/<std::pair<std::string, std::string> > listinfo22;
	listinfo22.push_back(std::pair<std::string, std::string>("preference_soft_compress_tex_none", "none"));
	listinfo22.push_back(std::pair<std::string, std::string>("preference_soft_compress_tex_halfline", "halfline"));
 	listinfo22.push_back(std::pair<std::string, std::string>("lz4", "lz4"));
 	listinfo22.push_back(std::pair<std::string, std::string>("lz4+TLG5", "lz4+tlg5"));
	SoftRendererOptPreference.Preferences.push_back(new tTVPPreferenceInfoSelectList("preference_software_compress_tex", "software_compress_tex", "none", listinfo22));



	OpenglOptPreference.Title = "preference_opengl_renderer_opt";
	OpenglOptPreference.Preferences = std::vector<iTVPPreferenceInfo*>();
	std::vector/*initializer_list*/<iTVPPreferenceInfo*> subs;
	subs.push_back(new tTVPPreferenceInfoConstant("preference_opengl_extension_desc"));
#ifdef CC_TARGET_OS_IPHONE
	subs.push_back(new tTVPPreferenceInfoCheckBox("GL_EXT_shader_framebuffer_fetch", "GL_EXT_shader_framebuffer_fetch", true));
#else
	subs.push_back(new tTVPPreferenceInfoCheckBox("GL_EXT_shader_framebuffer_fetch", "GL_EXT_shader_framebuffer_fetch", false));
#endif
	subs.push_back(new tTVPPreferenceInfoCheckBox("GL_ARM_shader_framebuffer_fetch", "GL_ARM_shader_framebuffer_fetch", true));
	subs.push_back(new tTVPPreferenceInfoCheckBox("GL_NV_shader_framebuffer_fetch", "GL_NV_shader_framebuffer_fetch", true));
	subs.push_back(new tTVPPreferenceInfoCheckBox("GL_EXT_copy_image", "GL_EXT_copy_image", false));
	subs.push_back(new tTVPPreferenceInfoCheckBox("GL_OES_copy_image", "GL_OES_copy_image", false));
	subs.push_back(new tTVPPreferenceInfoCheckBox("GL_ARB_copy_image", "GL_ARB_copy_image", false));
	subs.push_back(new tTVPPreferenceInfoCheckBox("GL_NV_copy_image", "GL_NV_copy_image", false));
	subs.push_back(new tTVPPreferenceInfoCheckBox("GL_EXT_clear_texture", "GL_EXT_clear_texture", true));
	subs.push_back(new tTVPPreferenceInfoCheckBox("GL_ARB_clear_texture", "GL_ARB_clear_texture", true));
	subs.push_back(new tTVPPreferenceInfoCheckBox("GL_QCOM_alpha_test", "GL_QCOM_alpha_test", true));
	OpenglOptPreference.Preferences.push_back(new tTVPPreferenceInfoSubPref("preference_opengl_extension_opt", subs));
	OpenglOptPreference.Preferences.push_back(new tTVPPreferenceInfoCheckBox("preference_ogl_accurate_render", "ogl_accurate_render", false));
//		new tTVPPreferenceInfoCheckBox("preference_opengl_dup_target", "ogl_dup_target", true),

	std::vector/*initializer_list*/<std::pair<std::string, std::string> > listinfo3;
	listinfo3.push_back(std::pair<std::string, std::string>("preference_ogl_texsize_auto", "0" ));
	listinfo3.push_back(std::pair<std::string, std::string>("preference_ogl_texsize_1024", "1024" ));
	listinfo3.push_back(std::pair<std::string, std::string>("preference_ogl_texsize_2048", "2048" ));
	listinfo3.push_back(std::pair<std::string, std::string>("preference_ogl_texsize_4096", "4096" ));
	listinfo3.push_back(std::pair<std::string, std::string>("preference_ogl_texsize_8192", "8192" ));
	listinfo3.push_back(std::pair<std::string, std::string>("preference_ogl_texsize_16384", "16384" ));
	OpenglOptPreference.Preferences.push_back(new tTVPPreferenceInfoSelectList("preference_ogl_max_texsize", "ogl_max_texsize", "0", listinfo3));

#if 0
	std::vector/*initializer_list*/<std::tuple<std::string, std::string, unsigned int> > listinfo2;
	listinfo2.push_back(std::make_tuple("preference_ogl_compress_tex_none", "none", 0));
	listinfo2.push_back(std::make_tuple("preference_ogl_compress_tex_half", "half", 0));
	listinfo2.push_back(std::make_tuple("ETC2", "etc2", 0x9278)); // GL_COMPRESSED_RGBA8_ETC2_EAC
	listinfo2.push_back(std::make_tuple("PVRTC", "pvrtc", 0x8C02)); // GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
	OpenglOptPreference.Preferences.push_back(new tTVPPreferenceInfoTextureCompressionSelectList("preference_ogl_compress_tex", "ogl_compress_tex", "none", listinfo2));
// 		new tTVPPreferenceInfoSelectList("preference_ogl_render_tex_quality", "ogl_render_tex_quality", "1", {
// 			{ "preference_ogl_render_tex_quality_100", "1" },
// 			{ "preference_ogl_render_tex_quality_75", "0.75" },
// 			{ "preference_ogl_render_tex_quality_50", "0.5" }
// 		}),
#else
	std::vector<my_tuple> listinfo2;
	listinfo2.push_back(my_tuple("preference_ogl_compress_tex_none", "none", 0));
	listinfo2.push_back(my_tuple("preference_ogl_compress_tex_half", "half", 0));
	listinfo2.push_back(my_tuple("ETC2", "etc2", 0x9278)); // GL_COMPRESSED_RGBA8_ETC2_EAC
	listinfo2.push_back(my_tuple("PVRTC", "pvrtc", 0x8C02)); // GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
	OpenglOptPreference.Preferences.push_back(new tTVPPreferenceInfoTextureCompressionSelectList("preference_ogl_compress_tex", "ogl_compress_tex", "none", listinfo2));
#endif


}
};
