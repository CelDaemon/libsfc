# SPDX-FileCopyrightText: 2025 CelDaemon <celdaemon@voidgroup.net>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(verify main.c
        main.c)
target_link_libraries(verify PUBLIC sfc)
