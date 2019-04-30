#pragma once

int find_files_by_content(const char* query, const char* dirpath, int block_size,
						  bool exclude_binary);