# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_executable(streaming_samples main.c)
target_link_libraries(streaming_samples PRIVATE zsa::zsa
    )
