#include "s_struct.h"
#include "f_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int S_check_struct_name_not_used_in_init(MRS_String *file_contents,
					 size_t struct_keyword_idx)
{
	int has_bracket = 0;
	int has_equal = 0;
	for (size_t i = struct_keyword_idx; i < file_contents->len; i++) {
		if (file_contents->value[i] ==
		    *tokens_to_str[F_CTOKENS_SEMI_COLON]) {
			break;
		}
		if (file_contents->value[i] ==
		    *tokens_to_str[F_CTOKENS_OPEN_CURLY]) {
			has_bracket = 1;
		}

		if (file_contents->value[i] ==
		    *tokens_to_str[F_CTOKENS_EQUALS]) {
			has_equal = 1;
		}
	}
	if (has_equal || !has_bracket) {
		return 1;
	}

	return 0;
}

MRS_String *S_get_struct_name(MRS_String *file_contents,
			      size_t struct_start_position)
{
	MRS_String open_curly;
	MRS_init(0, tokens_to_str[F_CTOKENS_OPEN_CURLY],
		 strlen(tokens_to_str[F_CTOKENS_OPEN_CURLY]), &open_curly);

	char *next_open_curly_position =
		MRS_strstr(file_contents, &open_curly, struct_start_position);

	MRS_free(&open_curly);

	MRS_String *name = MRS_create(MAX_STRUCT_NAME_LENGTH);
	char *start_of_name = &file_contents->value[struct_start_position] +
			      strlen(keywords_to_str[F_CKEYWORDS_STRUCT]);

	size_t name_length = next_open_curly_position - start_of_name;

	MRS_setstrn(name, start_of_name, name_length, name_length);
	MRS_remove_whitespace(name);

	if (name->len == 0) {
		MRS_free(name);
		free(name);
		return NULL;
	}

	MRS_shrink_to_fit(name);
	return name;
}

MRS_String *S_get_struct_typedef_name(MRS_String *file_contents,
				      size_t struct_start_position)
{
	MRS_String open_curly;
	MRS_init(0, tokens_to_str[F_CTOKENS_OPEN_CURLY],
		 strlen(tokens_to_str[F_CTOKENS_OPEN_CURLY]), &open_curly);

	MRS_String semi_colon;
	MRS_init(0, tokens_to_str[F_CTOKENS_SEMI_COLON],
		 strlen(tokens_to_str[F_CTOKENS_SEMI_COLON]), &semi_colon);

	char *struct_open_curly_ptr =
		MRS_strstr(file_contents, &open_curly, struct_start_position);

	size_t search_position =
		struct_open_curly_ptr - file_contents->value + 1;

	int bracket_stack = 1;
	size_t struct_end_bracket_position = 0;
	for (size_t i = search_position; i < file_contents->len; i++) {
		char current_char = MRS_get_char(file_contents, i);
		if (current_char == *tokens_to_str[F_CTOKENS_OPEN_CURLY]) {
			bracket_stack++;
		} else if (current_char ==
			   *tokens_to_str[F_CTOKENS_CLOSE_CURLY]) {
			bracket_stack--;
			if (bracket_stack == 0) {
				struct_end_bracket_position = i;
				break;
			}
		}
	}
	char *struct_semi_colon_ptr = MRS_strstr(file_contents, &semi_colon,
						 struct_end_bracket_position);

	MRS_free(&semi_colon);
	MRS_free(&open_curly);

	size_t struct_semi_colon_position = 0;
	MRS_get_idx(file_contents, struct_semi_colon_ptr,
		    &struct_semi_colon_position);
	size_t typedef_name_len =
		struct_semi_colon_position - struct_end_bracket_position - 1;

	MRS_String *name = MRS_create(MAX_STRUCT_NAME_LENGTH);

	MRS_setstrn(name,
		    &file_contents->value[struct_end_bracket_position + 1],
		    file_contents->len, typedef_name_len);
	MRS_remove_whitespace(name);

	if (name->len == 0) {
		MRS_free(name);
		free(name);
		return NULL;
	}

	MRS_shrink_to_fit(name);
	return name;
}

void S_destory_struct_variable(struct S_StructVariable *a)
{
	MRS_free(a->name);
	free(a->name);
}

void S_destroy_struct_info(struct S_StructInfo *a)
{
	if (a->name != NULL) {
		MRS_free(a->name);
		free(a->name);
	}
	if (a->typedef_name != NULL) {
		MRS_free(a->typedef_name);
		free(a->typedef_name);
	}

	MRS_free(a->filename);
	free(a->filename);

	for (size_t i = 0; i < a->variable_names_len; i++) {
		S_destory_struct_variable(&a->variable_names[i]);
	}
}

void S_append_struct_variable(MRS_String *file_contents,
			      size_t variable_start_idx, size_t variable_len,
			      struct S_StructInfo *dest)
{
	MRS_String *variable_name = malloc(sizeof(*variable_name));
	MRS_init(variable_len, &file_contents->value[variable_start_idx],
		 variable_len, variable_name);

	dest->variable_names[dest->variable_names_len].name = variable_name;
	dest->variable_names[dest->variable_names_len].line_number =
		F_get_line_number(file_contents, variable_start_idx);
	dest->variable_names_len++;
}

void S_add_struct_variables(MRS_String *file_contents, size_t struct_idx,
			    struct S_StructInfo *struct_info)
{
	MRS_String open_curly;
	MRS_init(0, tokens_to_str[F_CTOKENS_OPEN_CURLY],
		 strlen(tokens_to_str[F_CTOKENS_OPEN_CURLY]), &open_curly);

	char *open_curly_position =
		MRS_strstr(file_contents, &open_curly, struct_idx);

	size_t open_curly_idx;
	MRS_get_idx(file_contents, open_curly_position, &open_curly_idx);
	MRS_free(&open_curly);

	int bracket_stack = 1;
	struct_info->variable_names_len = 0;

	for (size_t i = open_curly_idx + 1; i < file_contents->len; i++) {
		char current_char = MRS_get_char(file_contents, i);

		if (current_char == *tokens_to_str[F_CTOKENS_OPEN_CURLY]) {
			bracket_stack++;
		} else if (current_char ==
			   *tokens_to_str[F_CTOKENS_CLOSE_CURLY]) {
			bracket_stack--;
			if (bracket_stack == 0) {
				break;
			}
		} else if (current_char ==
			   *tokens_to_str[F_CTOKENS_SEMI_COLON]) {
			size_t semi_colon_idx = i;
			size_t variable_name_end_idx = 0;
			size_t variable_name_len = 0;
			// search backwards from semi colon to end of variable name (to ignore whitespace)
			for (size_t j = semi_colon_idx - 1; j > struct_idx;
			     j--) {
				if (MRS_is_whitespace(file_contents, j) == 0) {
					continue;
				} else {
					variable_name_end_idx = j;
					break;
				}
			}

			for (size_t j = variable_name_end_idx; j > struct_idx;
			     j--) {
				if (F_is_valid_variable_char(
					    file_contents->value[j]) == 0) {
					variable_name_len++;
				} else {
					break;
				}
			}

			S_append_struct_variable(
				file_contents,
				variable_name_end_idx - variable_name_len + 1,
				variable_name_len, struct_info);
		}
	}

	return;
}

void S_get_struct_info(MRS_String *file_contents, MRS_String *filename,
		       size_t struct_idx, struct S_StructInfo *dest)
{
	MRS_String *name = S_get_struct_name(file_contents, struct_idx);
	MRS_String *typedef_name =
		S_get_struct_typedef_name(file_contents, struct_idx);

	dest->name = name;

	dest->typedef_name = typedef_name;

	dest->filename = MRS_create(filename->len);
	MRS_setstrn(dest->filename, filename->value, filename->len,
		    filename->len);

	dest->line_number = F_get_line_number(file_contents, struct_idx);

	S_add_struct_variables(file_contents, struct_idx, dest);
}

void S_seek_to_end_of_struct(MRS_String *file_contents,
			     size_t *current_position)
{
	if (S_check_struct_name_not_used_in_init(file_contents,
						 *current_position) != 0) {
		while (*current_position < file_contents->len) {
			if (file_contents->value[*current_position] ==
			    *tokens_to_str[F_CTOKENS_SEMI_COLON]) {
				return;
			}
			*current_position += 1;
		}
	}

	MRS_String open_curly;
	MRS_init(0, tokens_to_str[F_CTOKENS_OPEN_CURLY],
		 strlen(tokens_to_str[F_CTOKENS_OPEN_CURLY]), &open_curly);

	char *struct_open_curly_ptr =
		MRS_strstr(file_contents, &open_curly, *current_position);
	MRS_free(&open_curly);

	size_t open_curly_position = 0;
	MRS_get_idx(file_contents, struct_open_curly_ptr, &open_curly_position);
	size_t bracket_stack = 1;

	for (size_t i = open_curly_position + 1; i < file_contents->len; i++) {
		char current_char = MRS_get_char(file_contents, i);
		if (current_char == *tokens_to_str[F_CTOKENS_OPEN_CURLY]) {
			bracket_stack++;
		} else if (current_char ==
			   *tokens_to_str[F_CTOKENS_CLOSE_CURLY]) {
			bracket_stack--;
			if (bracket_stack == 0) {
				*current_position = i;
				return;
			}
		}
	}
}

void S_get_structs(MRS_String *file_contents, MRS_String *filename,
		   struct S_StructInfo structs_info[MAX_STRUCT_NAME_COUNT],
		   size_t *structs_len)
{
	memset(structs_info, 0,
	       sizeof(struct S_StructInfo) * MAX_STRUCT_NAME_COUNT);
	*structs_len = 0;

	size_t character_position = 0;
	while (character_position < file_contents->len) {
		int found = F_get_next_keyword_idx(file_contents,
						   character_position,
						   F_CKEYWORDS_STRUCT,
						   &character_position);
		if (found == -1) {
			return;
		}

		if (S_check_struct_name_not_used_in_init(
			    file_contents, character_position) == 0) {
			S_get_struct_info(file_contents, filename,
					  character_position,
					  &structs_info[*structs_len]);
			*structs_len += 1;
		}

		S_seek_to_end_of_struct(file_contents, &character_position);
	}
	return;
}
