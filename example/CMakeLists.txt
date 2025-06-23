# SPDX-FileCopyrightText: 2025 CelDaemon <celdaemon@voidgroup.net>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(example main.c
        main.c)
target_link_libraries(example PUBLIC sfc)
