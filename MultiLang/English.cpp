#include "MultiLang.h"

// ASCII

static constexpr const char* array_bounds_ex = "Blitz array index out of bounds.";
static constexpr const char* null_obj_ex = "Object does not exist!";
static constexpr const char* bad_data_type = "Bad data type! Type is not a float, string or an integer.";
static constexpr const char* out_of_data = "Out of data!";
static constexpr const char* stats_strings = "Active strings : {0}";
static constexpr const char* stats_objects = "Active objects : {0}";
static constexpr const char* stats_unreleased = "Unreleased objs: {0}";
static constexpr const char* sound_not_exist = "Sound does not exist";
static constexpr const char* channel_not_exist = "Channel does not exist";
static constexpr const char* illegal_sound_index = "Illegal sound index";
static constexpr const char* illegal_channel_index = "Illegal channel index";
static constexpr const char* bank_not_exist = "bbBank does not exist";
static constexpr const char* offset_out_of_range = "Offset out of range";
static constexpr const char* graphics_not_set = "3D Graphics mode not set.";
static constexpr const char* texture_not_exist = "Texture does not exist!";
static constexpr const char* brush_not_exist = "Brush does not exist!";
static constexpr const char* entity_not_exist = "Entity does not exist!";
static constexpr const char* parent_entity_not_exist = "Parent entity does not exist.";
static constexpr const char* entity_not_mesh = "Entity is not a mesh!";
static constexpr const char* entity_not_object = "Entity is not an object!";
static constexpr const char* collision_out_of_range = "Collision index out of range.";
static constexpr const char* entity_not_camera = "Entity is not a camera!";
static constexpr const char* entity_not_light = "Entity is not a light!";
static constexpr const char* entity_not_model = "Entity is not a model!";
static constexpr const char* entity_not_sprite = "Entity is not a sprite!";
static constexpr const char* entity_not_md2_model = "Entity is not an MD2 Model!";
static constexpr const char* entity_not_bsp_model = "Entity is not a BSP Model!";
static constexpr const char* entity_not_terrain = "Entity is not a terrain!";
static constexpr const char* illegal_number_segments = "Illegal number of segments!";
static constexpr const char* vertex_out_of_range = "Vertex index out of range.";
static constexpr const char* texture_coordinate_out_of_range = "Texture coordinate set out of range.";
static constexpr const char* illegal_texture_frames = "Illegal number of texture frames!";
static constexpr const char* mesh_cannot_add_to_self = "A mesh cannot be added to itself!";
static constexpr const char* surface_out_of_range = "Surface Index out of range.";
static constexpr const char* illegal_terrain_size = "Illegal terrain size!";
static constexpr const char* unable_load_heightmap = "Unable to load heightmap image.";
static constexpr const char* terrain_must_be_square = "Terrain must be square!";
static constexpr const char* listener_already_created = "Listener already created!";
static constexpr const char* no_listener_created = "No Listener created.";
static constexpr const char* entity_cannot_parented_itself = "Entity cannot be parented to itself!";
static constexpr const char* entity_no_animations = "Entity has no animations.";
static constexpr const char* entity_not_model_or_camera = "Entity is not a model or camera!";
static constexpr const char* entitytype_id_range = "EntityType ID must be a number from 0 to 999.";
static constexpr const char* unable_create_gxscene_instance = "Unable to create gxScene instance!";
static constexpr const char* file_not_exist = "File does not exist";
static constexpr const char* directory_not_exist = "Directory does not exist";
static constexpr const char* image_not_exist = "Image doesn't exist.";
static constexpr const char* image_frame_out_of_range = "Image frame out of range!";
static constexpr const char* font_not_exist = "Font doesn't exist.";
static constexpr const char* buffer_not_exist = "Buffer doesn't exist.";
static constexpr const char* illegal_graphics_driver_index = "Illegal graphics driver index.";
static constexpr const char* illegal_graphics_mode_index = "Illegal graphics mode index.";
static constexpr const char* unable_create_gxgraphics_instance = "Unable to create a gxGraphics instance.";
static constexpr const char* illegal_graphics_mode = "Illegal Graphics mode. Graphics modes are 0, 1, 2, 3, 4, 6 and 7.";
static constexpr const char* illegal_graphics3d_mode = "Illegal Graphics3D mode. Graphics modes are 0, 1, 2, 3, 4, 6 and 7.";
static constexpr const char* unable_close_gxgraphics_instance = "Unable to close gxGraphics instance.";
static constexpr const wchar_t* runtime_error = L"Catastrophic Error!";
static constexpr const char* illegal_frame_count = "Illegal frame count!";
static constexpr const char* illegal_first_frame = "Illegal first frame!";
static constexpr const char* not_enough_frames_bitmap = "Not enough frames in bitmap.";
static constexpr const char* udp_stream_not_exist = "UDP Stream does not exist";
static constexpr const char* tcp_stream_not_exist = "TCP Stream does not exist";
static constexpr const char* tcp_server_not_exist = "TCP Server does not exist";
static constexpr const char* host_out_of_range = "Host index out of range";
static constexpr const char* stream_not_exist = "Stream does not exist";
static constexpr const char* readbyte_invalid_byte = "ReadByte couldn't read a valid byte.\nMake sure the file you attempted to read isn't corrupted.";
static constexpr const char* readshort_invalid_short = "ReadShort couldn't read a valid short.\nMake sure the file you attempted to read isn't corrupted.";
static constexpr const char* readint_invalid_int = "ReadInt couldn't read a valid integer.\nMake sure the file you attempted to read isn't corrupted.";
static constexpr const char* readfloat_invalid_float = "ReadFloat couldn't read a valid float.\nMake sure the file you attempted to read isn't corrupted.";
static constexpr const char* readstring_invalid_string = "ReadString couldn't read a valid string.\nMake sure the file you attempted to read isn't corrupted.";
static constexpr const char* illegal_buffer_size = "Illegal buffer size";
static constexpr const char* string_parameter_positive = "parameter must be positive";
static constexpr const char* string_parameter_greater = "parameter must be greater than 0";
static constexpr const char* userlib_not_found = "User lib not found: {0}";
static constexpr const char* userlib_function_not_found = "User lib function not found: {0}";
static constexpr const wchar_t* integer_divide_zero = L"Integer divide by zero.";
static constexpr const char* memory_access_violation = "Memory Access Violation!\nThe program attempted to read or write to a protected memory address.";
static constexpr const wchar_t* illegal_instruction = L"Illegal instruction.\nProcess tried to execute an invalid CPU instruction.";
static constexpr const wchar_t* stack_overflow = L"Stack overflow.\nMake sure there is no recursion without a base case.";
static constexpr const wchar_t* integer_overflow = L"Integer overflow!\nMake sure the integer doesnt exceed a value of 2147483647.";
static constexpr const wchar_t* float_overflow = L"Float overflow!\nMake sure the float doesn't exceed a value of 3.40282347e+38F.";
static constexpr const wchar_t* float_divide_zero = L"Float divide by zero.";
static constexpr const wchar_t* unknown_runtime_exception = L"Unknown runtime exception.";
static constexpr const char* unable_run_module = "Unable to run Blitz Basic module.";
static constexpr const char* cant_find_symbol = "Can't find symbol: {0}";
static constexpr const char* unknown_exception_thrown = "Unknown/non-standard exception thrown!";
static constexpr const char* startup_error = "Startup Error: {0}";
static constexpr const char* array_index_out_of_bounds = "Array index out of bounds.";
static constexpr const char* unable_start_program = "Unable to start program! A required module could not be started.";
static constexpr const char* expect_identifier = "expecting identifier after \".\"";
static constexpr const char* expect_string_afrer_directive = "expecting string after lib directive";
static constexpr const char* unknown_decl_directive = "unknown decl directive";
static constexpr const char* function_decl_without_directive = "function decl without lib directive";
static constexpr const char* duplicate_identifier = "duplicate identifier";
static constexpr const char* expect_left_bracket_after_function_identifier = "expecting \"(\" after function identifier";
static constexpr const char* expect_right_bracket_after_function_identifier = "expecting \")\" after function decl";
static constexpr const char* expect_identifier_or_string_after_alias = "expecting identifier or string after alias";
static constexpr const char* unable_open_linker_dll = "Unable to open linker.dll";
static constexpr const char* error_in_linker_dll = "Error in linker.dll";
static constexpr const char* unable_open_runtime_dll = "Unable to open runtime.dll";
static constexpr const char* error_in_runtime_dll = "Error in runtime.dll";
static constexpr const char* library_version_error = "Library version error";
static constexpr const char* immediate_value_cannot_by_label = "immediate value cannot by a label";
static constexpr const char* operand_must_be_immediate = "operand must be immediate";
static constexpr const char* unrecognized_instruction = "unrecognized instruction";
static constexpr const char* illegal_addressing_mode = "illegal addressing mode";
static constexpr const char* operand_error = "operand error";
static constexpr const char* duplicate_label = "duplicate label";
static constexpr const char* missing_close_quote = "missing close quote";
static constexpr const char* expect_comma = "expecting \",\"";
static constexpr const char* too_many_operands = "Too many operands";
static constexpr const char* expression_must_be_constant = "Expression must be constant";
static constexpr const char* constants_must_initialized = "Constants must be initialized";
static constexpr const char* duplicate_variable_name = "Duplicate variable name";
static constexpr const char* undefined_label = "Undefined label";
static constexpr const char* data_expression_must_be_constant = "Data expression must be constant";
static constexpr const char* blitz_array_sizes_must_be_constant = "Blitz array sizes must be constant";
static constexpr const char* blitz_array_sizes_must_not_negative = "Blitz array sizes must not be negative";
static constexpr const char* illegal_type_conversion = "Illegal type conversion ({0} -> {1})";
static constexpr const char* too_many_parameters = "Too many parameters";
static constexpr const char* not_enough_parameters = "Not enough parameters";
static constexpr const char* function_not_found = "Function \"{0}\" not found";
static constexpr const char* incorrect_function_return_type = "incorrect function return type";
static constexpr const char* cant_convert_null_to_int = "Can't convert null to int";
static constexpr const char* cant_convert_null_to_float = "Can't convert null to float";
static constexpr const char* cant_convert_null_to_string = "Can't convert null to string";
static constexpr const char* illegal_operator_for_type = "Illegal operator for type";
static constexpr const char* arithmetic_operator_custom_type = "Arithmetic operator cannot be applied to custom type objects";
static constexpr const char* operator_cannot_applied_to_strings = "Operator cannot be applied to strings";
static constexpr const char* division_by_zero = "Division by zero.";
static constexpr const char* custom_type_not_found = "custom type name not found";
static constexpr const char* type_is_not_custom_type = "type is not a custom type";
static constexpr const char* after_cannot_used_on_null = "\"After\" cannot be used on \"Null\"";
static constexpr const char* after_must_used_with_custom_type = "\"After\" must be used with a custom type object";
static constexpr const char* before_cannot_used_with_null = "\"Before\" cannot be used with \"Null\"";
static constexpr const char* before_must_used_with_custom_type = "\"Before\" must be used with a custom type object";
static constexpr const char* objecthandle_must_used_with_object = "\"ObjectHandle\" must be used with an object";
static constexpr const char* internal_compiler_error = "INTERNAL COMPILER ERROR";
static constexpr const char* type_not_found = "Type \"{0}\" not found";
static constexpr const char* error_in_operand = "error in operand";
static constexpr const char* illegal_operand_size = "illegal operand size";
static constexpr const char* register_must_be_32_bit = "register must be 32 bit";
static constexpr const char* next_without_for = "\"Next\" without \"For\"";
static constexpr const char* wend_without_while = "\"Wend\" without \"While\"";
static constexpr const char* else_without_if = "\"Else\" without \"If\"";
static constexpr const char* endif_without_if = "\"Endif\" without \"If\"";
static constexpr const char* end_function_without_function = "\"End Function\" without \"Function\"";
static constexpr const char* until_without_repeat = "\"Until\" without \"Repeat\"";
static constexpr const char* forever_without_repeat = "\"Forever\" without \"Repeat\"";
static constexpr const char* case_without_select = "\"Case\" without \"Select\"";
static constexpr const char* end_select_without_select = "\"End Select\" without \"Select\"";
static constexpr const char* expecting = "Expecting {0}";
static constexpr const char* identifier = "identifier";
static constexpr const char* include_filename = "include filename";
static constexpr const char* unable_open_include_file = "Unable to open include file";
static constexpr const char* end_of_file = "end-of-file";
static constexpr const char* mismatched_brackets = "Mismatched brackets";
static constexpr const char* variable_assignment = "variable assignment";
static constexpr const char* until_or_forever = "\"Until\" or \"Forever\"";
static constexpr const char* expression_sequence = "expression sequence";
static constexpr const char* case_default_or_end_select = "\"Case\", \"Default\" or \"End Select\"";
static constexpr const char* before_or_after = "\"Before\" or \"After\"";
static constexpr const char* data_can_only_appear_in_main = "\"Data\" can only appear in main program";
static constexpr const char* type_can_only_appear_in_main = "\"Type\" can only appear in main program";
static constexpr const char* const_can_only_appear_in_main = "\"Const\" can only appear in main program";
static constexpr const char* function_can_only_appear_in_main = "\"Function\" can only appear in main program";
static constexpr const char* global_can_only_appear_in_main = "\"Global\" can only appear in main program";
static constexpr const char* blitz_arrays_may_not_be_constant = "Blitz arrays may not be constant";
static constexpr const char* cant_have_zero_dimension_array = "can\"t have a 0 dimensional array";
static constexpr const char* field_or_end_type = "\"Field\" or \"End Type\"";
static constexpr const char* expression = "expression";
static constexpr const char* undefined_label_check = "Undefined label \"{0}\"";
static constexpr const char* array_not_found_in_main = "Array not found in main program";
static constexpr const char* constants_can_not_assigned_to = "Constants can not be assigned to";
static constexpr const char* blitz_arrays_can_not_assigned_to = "Blitz arrays can not be assigned to";
static constexpr const char* gosub_may_not_used_inside_function = "\"Gosub\" may not be used inside a function";
static constexpr const char* break_must_appear_inside_loop = "break must appear inside a loop";
static constexpr const char* index_variable_can_not_constant = "Index variable can not be constant";
static constexpr const char* index_variable_must_integer_or_real = "index variable must be integer or real";
static constexpr const char* step_value_must_constant = "Step value must be constant";
static constexpr const char* index_variable_is_not_newtype = "Index variable is not a NewType";
static constexpr const char* type_name_not_found = "Type name not found";
static constexpr const char* type_mismatch = "Type mismatch";
static constexpr const char* main_cannot_return_value = "Main program cannot return a value";
static constexpr const char* cant_delete_non_newtype = "Can't delete non-Newtype";
static constexpr const char* specified_name_is_not_newtype_name = "Specified name is not a NewType name";
static constexpr const char* illegal_expression_type = "Illegal expression type";
static constexpr const char* objects_types_are_different = "Objects types are different";
static constexpr const char* select_cannot_used_with_objects = "Select cannot be used with objects";
static constexpr const char* constants_can_not_modified = "Constants can not be modified";
static constexpr const char* data_can_not_read_into_object = "Data can not be read into an object";
static constexpr const char* variable_type_mismatch = "Variable type mismatch";
static constexpr const char* identifier_not_used_like_this = "Identifier \"{0}\" may not be used like this";
static constexpr const char* array_not_found = "Array not found";
static constexpr const char* array_type_mismatch = "array type mismatch";
static constexpr const char* incorrect_number_of_dimensions = "incorrect number of dimensions";
static constexpr const char* variable_must_be_type = "Variable must be a Type";
static constexpr const char* type_field_not_found = "Type field not found";
static constexpr const char* variable_must_a_blitz_array = "Variable must be a Blitz array";
static constexpr const char* incorrect_number_of_subscripts = "Incorrect number of subscripts";
static constexpr const char* blitz_array_subscript_out_of_range = "Blitz array subscript out of range";
static constexpr const char* reserved_pseudo_namespace = "Reserved pseudo namespace.";

static constexpr const wchar_t* runtime_message = L"Runtime Message";
static constexpr const char* current_language = "Current Language: English";
static constexpr const char* debugger_locals = "Locals";
static constexpr const char* debugger_globals = "Globals";
static constexpr const char* debugger_consts = "Consts";
static constexpr const char* debugger_unknown = "<unknown>";
static constexpr const char* debugger_cant_access_appdata = "Couldn't access the AppData folder! This is needed for the preferences file to work.\nThe IDE will use the default values.";
static constexpr const char* debugger_cant_create_folder = "Couldn't create a folder for the preferences!\nThe IDE will use the default values.";
static constexpr const char* debugger_empty_ini = "blitzide.ini is empty!\nDefaults will be set.";
static constexpr char credits[] =
"Programming and design: Mark Sibly\r\n"
"Keeping this thing alive: juanjp600, Saalvage, VaneBrain, AestheticalZ and ZiYueCommentary\r\n"
".INI loading using a modified version of \"IniPP\", originally by Matthias C. M. Troffaes\r\n"
"Documentation: Mark Sibly, Simon Harrison, Paul Gerfen, Shane Monroe and the Blitz Doc Team\r\n"
"Testing and support: James Boyd, Simon Armstrong and the Blitz Dev Team\r\n"
"Image loader courtesy of FreeImage by Floris van den berg";
static constexpr const char* title_release = " - Release Version";
static constexpr const char* program_ended = "Program has ended.";
static constexpr const char* bb_err = "Blitz Error";
static constexpr const char* init_err = "Error when creating BlitzIDE process. Please make sure ide.exe is inside the \"bin\" folder.";
static constexpr const char* blitz3d_message = "Blitz3D Message";
static constexpr const char* created_with_beta = "Created with Blitz3D Beta V{0}.{1}";
static constexpr const char* invalid_macro_name = "Invalid macro name: {0}";
static constexpr const char* redefinition_of_macro = "Redefinition of macro: {0}";
static constexpr const char* error_evaluating_condition = "Error evaluating condition: {0}";
static constexpr const char* elseif_without_if = "#elseif_ without matching #if_";
static constexpr const char* else_without_if_macro = "#else_ without matching #if_";
static constexpr const char* macro_exceeded = "Macro expansion depth exceeded";
static constexpr const char* unsupported_operator = "Unsupported operator: {0}";
static constexpr const char* expression_evaluation_resulted_in_no_value = "Expression evaluation resulted in no value";
static constexpr const char* invalid_macro_string = "Invalid macro string";