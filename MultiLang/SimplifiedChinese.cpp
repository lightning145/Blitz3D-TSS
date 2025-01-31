#include "MultiLang.h"

// GB 2312

static constexpr const char* array_bounds_ex = "Blitz数组索引越界";
static constexpr const char* null_obj_ex = "对象不存在";
static constexpr const char* bad_data_type = "错误的数据类型！数据类型不应是浮点值、字符串或整型";
static constexpr const char* out_of_data = "数据不足";
static constexpr const char* stats_strings = "激活的字符串：{0}";
static constexpr const char* stats_objects = "激活的对象：{0}";
static constexpr const char* stats_unreleased = "未释放的对象：{0}";
static constexpr const char* sound_not_exist = "声音不存在";
static constexpr const char* channel_not_exist = "频道不存在";
static constexpr const char* illegal_sound_index = "非法声音索引";
static constexpr const char* illegal_channel_index = "非法声音频道索引";
static constexpr const char* bank_not_exist = "bbBank不存在";
static constexpr const char* offset_out_of_range = "位置超出范围";
static constexpr const char* graphics_not_set = "3D画面模式未设置";
static constexpr const char* texture_not_exist = "纹理不存在";
static constexpr const char* brush_not_exist = "Brush不存在";
static constexpr const char* entity_not_exist = "实体不存在";
static constexpr const char* parent_entity_not_exist = "父实体不存在";
static constexpr const char* entity_not_mesh = "实体不是Mesh";
static constexpr const char* entity_not_object = "实体不是对象";
static constexpr const char* collision_out_of_range = "碰撞索引超出范围";
static constexpr const char* entity_not_camera = "实体不是相机";
static constexpr const char* entity_not_light = "实体不是灯光";
static constexpr const char* entity_not_model = "实体不是模型";
static constexpr const char* entity_not_sprite = "实体不是Sprite";
static constexpr const char* entity_not_md2_model = "实体不是MD2模型";
static constexpr const char* entity_not_bsp_model = "实体不是BSP模型";
static constexpr const char* entity_not_terrain = "实体不是地形";
static constexpr const char* illegal_number_segments = "段数非法";
static constexpr const char* vertex_out_of_range = "顶点索引超出范围";
static constexpr const char* texture_coordinate_out_of_range = "纹理坐标设置超出范围";
static constexpr const char* illegal_texture_frames = "非法纹理帧数";
static constexpr const char* mesh_cannot_add_to_self = "无法将Mesh添加到其本身";
static constexpr const char* surface_out_of_range = "表面索引超出范围";
static constexpr const char* illegal_terrain_size = "非法地形大小";
static constexpr const char* unable_load_heightmap = "无法加载高度图";
static constexpr const char* terrain_must_be_square = "地表必须为正方形";
static constexpr const char* listener_already_created = "已创建监听器";
static constexpr const char* no_listener_created = "未创建监听器";
static constexpr const char* entity_cannot_parented_itself = "实体不能为其自身的父实体";
static constexpr const char* entity_no_animations = "实体无动画";
static constexpr const char* entity_not_model_or_camera = "实体不是模型或相机";
static constexpr const char* entitytype_id_range = "EntityType ID必须为0至999之间的数字";
static constexpr const char* unable_create_gxscene_instance = "无法创建gxScene实例";
static constexpr const char* file_not_exist = "文件不存在";
static constexpr const char* directory_not_exist = "文件夹不存在";
static constexpr const char* image_not_exist = "图像不存在";
static constexpr const char* image_frame_out_of_range = "图像帧超出范围";
static constexpr const char* font_not_exist = "字体不存在";
static constexpr const char* buffer_not_exist = "缓冲区不存在";
static constexpr const char* illegal_graphics_driver_index = "非法显示驱动索引";
static constexpr const char* illegal_graphics_mode_index = "非法显示模式索引";
static constexpr const char* unable_create_gxgraphics_instance = "无法创建gxGraphics实例";
static constexpr const char* illegal_graphics_mode = "非法显示模式，显示模式为0到7之间的值。";
static constexpr const char* illegal_graphics3d_mode = "非法3D显示模式，显示模式为0到7之间的值。";
static constexpr const char* unable_close_gxgraphics_instance = "无法关闭gxGraphics实例";
static constexpr const wchar_t* runtime_error = L"运行时错误！";
static constexpr const char* illegal_frame_count = "非法帧数";
static constexpr const char* illegal_first_frame = "非法第一帧";
static constexpr const char* not_enough_frames_bitmap = "位图帧数不足";
static constexpr const char* udp_stream_not_exist = "UDP流不存在";
static constexpr const char* tcp_stream_not_exist = "TCP流不存在";
static constexpr const char* tcp_server_not_exist = "TCP服务器不存在";
static constexpr const char* host_out_of_range = "主机索引超出范围";
static constexpr const char* stream_not_exist = "流不存在";
static constexpr const char* readbyte_invalid_byte = "ReadByte无法读取有效字节\n请确保读取的文件尚未损坏";
static constexpr const char* readshort_invalid_short = "ReadShort无法读取有效短整型\n请确保读取的文件尚未损坏";
static constexpr const char* readint_invalid_int = "ReadInt无法读取有效整型\n请确保读取的文件尚未损坏";
static constexpr const char* readfloat_invalid_float = "ReadFloat无法读取有效浮点值\n请确保读取的文件尚未损坏";
static constexpr const char* readstring_invalid_string = "ReadString无法读取有效字符串\n请确保读取的文件尚未损坏";
static constexpr const char* illegal_buffer_size = "非法缓冲区大小";
static constexpr const char* string_parameter_positive = "参数必须为正";
static constexpr const char* string_parameter_greater = "参数必须大于0";
static constexpr const char* userlib_not_found = "用户库未找到：{0}";
static constexpr const char* userlib_function_not_found = "用户库函数未找到：{0}";
static constexpr const wchar_t* integer_divide_zero = L"整数除零";
static constexpr const char* memory_access_violation = "内存访问冲突！\n程序试图读取或写入受保护的内存地址。";
static constexpr const wchar_t* illegal_instruction = L"非法指令。\n程序试图执行无效的CPU指令";
static constexpr const wchar_t* stack_overflow = L"堆栈溢出！\n请确保程序内没有递归现象";
static constexpr const wchar_t* integer_overflow = L"整数溢出！\n请确保整数的值不超过2147483647";
static constexpr const wchar_t* float_overflow = L"浮点溢出！\n请确保浮点的值不超过3.40282347e+38F";
static constexpr const wchar_t* float_divide_zero = L"浮点除零";
static constexpr const wchar_t* unknown_runtime_exception = L"未知运行时错误";
static constexpr const char* unable_run_module = "无法启动Blitz Basic模块";
static constexpr const char* cant_find_symbol = "找不到符号：{0}";
static constexpr const char* unknown_exception_thrown = "未知/非标准异常抛出！";
static constexpr const char* startup_error = "启动错误：{0}";
static constexpr const char* array_index_out_of_bounds = "数组索引超出范围";
static constexpr const char* unable_start_program = "无法启动程序！无法启动所需模块";
static constexpr const char* reserved_pseudo_namespace = "保留的伪命名空间";
static constexpr const char* expect_identifier = "\".\"后应为标识符";
static constexpr const char* expect_string_afrer_directive = "lib指令后应为字符串";
static constexpr const char* unknown_decl_directive = "未知库指令";
static constexpr const char* function_decl_without_directive = "无lib的函数库";
static constexpr const char* duplicate_identifier = "重复标识符";
static constexpr const char* expect_left_bracket_after_function_identifier = "函数名后缺少\"(\"";
static constexpr const char* expect_right_bracket_after_function_identifier = "函数参数后缺少\")\"";
static constexpr const char* expect_identifier_or_string_after_alias = "别名后应为标识符或字符串";
static constexpr const char* unable_open_linker_dll = "无法打开linker.dll";
static constexpr const char* error_in_linker_dll = "linker.dll出错";
static constexpr const char* unable_open_runtime_dll = "无法打开runtime.dll";
static constexpr const char* error_in_runtime_dll = "runtime.dll出错";
static constexpr const char* library_version_error = "库版本出错";
static constexpr const char* immediate_value_cannot_by_label = "立即值不能通过标签";
static constexpr const char* operand_must_be_immediate = "操作数必须为立即数";
static constexpr const char* unrecognized_instruction = "未识别指令";
static constexpr const char* illegal_addressing_mode = "非法寻地址方式";
static constexpr const char* operand_error = "操作数误差";
static constexpr const char* duplicate_label = "重复标签";
static constexpr const char* missing_close_quote = "缺失后引号";
static constexpr const char* expect_comma = "缺少\",\"";
static constexpr const char* too_many_operands = "操作数过多";
static constexpr const char* expression_must_be_constant = "表达式必须为常数";
static constexpr const char* constants_must_initialized = "常数必须被初始化";
static constexpr const char* duplicate_variable_name = "重复变量名";
static constexpr const char* undefined_label = "未定义标签";
static constexpr const char* data_expression_must_be_constant = "数据表达式必须为常数";
static constexpr const char* blitz_array_sizes_must_be_constant = "Blitz数组大小必须为常数";
static constexpr const char* blitz_array_sizes_must_not_negative = "Blitz数组大小不能为负数";
static constexpr const char* illegal_type_conversion = "非法类型转换（{0} -> {1}）";
static constexpr const char* too_many_parameters = "参数过多";
static constexpr const char* not_enough_parameters = "参数过少";
static constexpr const char* function_not_found = "函数\"{0}\"未找到";
static constexpr const char* incorrect_function_return_type = "错误函数返回值";
static constexpr const char* cant_convert_null_to_int = "不能将null转为整数";
static constexpr const char* cant_convert_null_to_float = "不能将null转为浮点值";
static constexpr const char* cant_convert_null_to_string = "不能将null转为字符串";
static constexpr const char* illegal_operator_for_type = "非法类型运算符";
static constexpr const char* arithmetic_operator_custom_type = "算术运算符不能用于自定类型对象";
static constexpr const char* operator_cannot_applied_to_strings = "运算符不能用于字符串";
static constexpr const char* division_by_zero = "数字除零";
static constexpr const char* custom_type_not_found = "自定类型名未找到";
static constexpr const char* type_is_not_custom_type = "类型不是自定类型";
static constexpr const char* after_cannot_used_on_null = "\"After\"不能用于\"Null\"";
static constexpr const char* after_must_used_with_custom_type = "\"After\"必须和自定类型对象使用";
static constexpr const char* before_cannot_used_with_null = "\"Before\"不能用于\"Null\"";
static constexpr const char* before_must_used_with_custom_type = "\"Before\"必须和自定类型对象使用";
static constexpr const char* objecthandle_must_used_with_object = "\"ObjectHandle\"必须和对象使用";
static constexpr const char* internal_compiler_error = "编译器内部错误！";
static constexpr const char* type_not_found = "类型\"{0}\"未找到";
static constexpr const char* error_in_operand = "操作数错误";
static constexpr const char* illegal_operand_size = "非法操作数大小";
static constexpr const char* register_must_be_32_bit = "寄存器必须为32位";
static constexpr const char* next_without_for = "\"Next\"缺少\"For\"";
static constexpr const char* wend_without_while = "\"Wend\"缺少\"While\"";
static constexpr const char* else_without_if = "\"Else\"缺少\"If\"";
static constexpr const char* endif_without_if = "\"Endif\"缺少\"If\"";
static constexpr const char* end_function_without_function = "\"End Function\"缺少\"Function\"";
static constexpr const char* until_without_repeat = "\"Until\"缺少\"Repeat\"";
static constexpr const char* forever_without_repeat = "\"Forever\"缺少\"Repeat\"";
static constexpr const char* case_without_select = "\"Case\"缺少\"Select\"";
static constexpr const char* end_select_without_select = "\"End Select\"缺少\"Select\"";
static constexpr const char* expecting = "缺少{0}";
static constexpr const char* identifier = "标识符";
static constexpr const char* include_filename = "导入文件名";
static constexpr const char* unable_open_include_file = "无法打开导入文件";
static constexpr const char* end_of_file = "文件尾部（EOF）";
static constexpr const char* mismatched_brackets = "括号不匹配";
static constexpr const char* variable_assignment = "变量赋值";
static constexpr const char* until_or_forever = "\"Until\"或\"Forever\"";
static constexpr const char* expression_sequence = "表达式序列";
static constexpr const char* case_default_or_end_select = "\"Case\"、\"Default\"或\"End Select\"";
static constexpr const char* before_or_after = "\"Before\"或\"After\"";
static constexpr const char* data_can_only_appear_in_main = "\"Data\"只能在主程序出现";
static constexpr const char* type_can_only_appear_in_main = "\"Type\"只能在主程序出现";
static constexpr const char* const_can_only_appear_in_main = "\"Const\"只能在主程序出现";
static constexpr const char* function_can_only_appear_in_main = "\"Function\"只能在主程序出现";
static constexpr const char* global_can_only_appear_in_main = "\"Global\"只能在主程序出现";
static constexpr const char* blitz_arrays_may_not_be_constant = "Blitz数组不能为常量";
static constexpr const char* cant_have_zero_dimension_array = "不能有零维数组";
static constexpr const char* field_or_end_type = "\"Field\"或\"End Type\"";
static constexpr const char* expression = "表达式";
static constexpr const char* undefined_label_check = "未声明标签\"{0}\"";
static constexpr const char* array_not_found_in_main = "在主程序中找不到数组";
static constexpr const char* constants_can_not_assigned_to = "不能将常量分配给";
static constexpr const char* blitz_arrays_can_not_assigned_to = "不能将Blitz数组分配给";
static constexpr const char* gosub_may_not_used_inside_function = "\"Gosub\"不能在函数内使用";
static constexpr const char* break_must_appear_inside_loop = "break必须在循环中出现";
static constexpr const char* index_variable_can_not_constant = "索引变量不能为常量";
static constexpr const char* index_variable_must_integer_or_real = "索引变量必须为整数或实数";
static constexpr const char* step_value_must_constant = "步进值必须为常量";
static constexpr const char* index_variable_is_not_newtype = "索引变量不是新类型";
static constexpr const char* type_name_not_found = "类型名未找到";
static constexpr const char* type_mismatch = "类型不匹配";
static constexpr const char* main_cannot_return_value = "主程序不能返回值";
static constexpr const char* cant_delete_non_newtype = "不能删除非新类型";
static constexpr const char* specified_name_is_not_newtype_name = "指定名称不是新类型名";
static constexpr const char* illegal_expression_type = "非法表达式类型";
static constexpr const char* objects_types_are_different = "对量类型不同";
static constexpr const char* select_cannot_used_with_objects = "Select不能用于对象";
static constexpr const char* constants_can_not_modified = "常量不可被修改";
static constexpr const char* data_can_not_read_into_object = "无法将数据读入Object";
static constexpr const char* variable_type_mismatch = "变量类型不匹配";
static constexpr const char* identifier_not_used_like_this = "标识符\"{0}\"不允许如此使用";
static constexpr const char* array_not_found = "数组未找到";
static constexpr const char* array_type_mismatch = "数组类型不匹配";
static constexpr const char* incorrect_number_of_dimensions = "错误维度数字";
static constexpr const char* variable_must_be_type = "变量必须为类型";
static constexpr const char* type_field_not_found = "类型数据未找到";
static constexpr const char* variable_must_a_blitz_array = "变量必须为Blitz数组";
static constexpr const char* incorrect_number_of_subscripts = "下标数出错";
static constexpr const char* blitz_array_subscript_out_of_range = "Blitz数组下标超出范围";

static constexpr const wchar_t* runtime_message = L"运行时消息";
static constexpr const char* current_language = "当前语言：简体中文";
static constexpr const char* debugger_locals = "本地";
static constexpr const char* debugger_globals = "全局";
static constexpr const char* debugger_consts = "常量";
static constexpr const char* debugger_unknown = "<未知>";
static constexpr const char* debugger_cant_access_appdata = "无法访问AppData文件夹！配置文件无法读取。\nIDE将使用默认值";
static constexpr const char* debugger_cant_create_folder = "无法为配置文件创建文件夹！\nIDE将使用默认值";
static constexpr const char* debugger_empty_ini = "blitzide.ini为空！\n将使用默认设置";
static constexpr char credits[] =
"编程及设计：Mark Sibly\r\n"
"项目续命：juanjp600、Saalvage、VaneBrain、AestheticalZ和ZiYueCommentary\r\n"
".INI加载器使用\"IniPP\"的修改版本，原版由Matthias C. M. Troffaes制作\r\n"
"文档：Mark Sibly、Simon Harrison、Paul Gerfen、Shane Monroe和Blitz文档撰写团队\r\n"
"测试及支持：James Boyd、Simon Armstrong和Blitz开发团队\r\n"
"图像加载器：Floris van den berg制作的FreeImage";
static constexpr const char* title_release = " - 发布版本";
static constexpr const char* program_ended = "程序已结束";
static constexpr const char* bb_err = "Blitz错误";
static constexpr const char* init_err = "启动BlitzIDE时出错。请检查\"bin\"文件夹内是否有ide.exe。";
static constexpr const char* blitz3d_message = "Blitz3D消息";
static constexpr const char* created_with_beta = "使用Blitz3D Beta V{0}.{1}制作";
static constexpr const char* invalid_macro_name = "无效宏名称：{0}";
static constexpr const char* redefinition_of_macro = "重复宏名称：{0}";
static constexpr const char* error_evaluating_condition = "错误宏运算语句：{0}";
static constexpr const char* elseif_without_if = "#elseif_缺少#if_";
static constexpr const char* else_without_if_macro = "#else_缺少#if_";
static constexpr const char* macro_exceeded = "宏深度达到限制";
static constexpr const char* unsupported_operator = "无效操作符：{0}";
static constexpr const char* expression_evaluation_resulted_in_no_value = "表达式没有结果";