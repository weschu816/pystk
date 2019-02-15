/*
 * Copyright © 2013 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <stdio.h>

#include "wgl_common.h"
#include <glproxy/gl.h>

static int
test_function(HDC hdc)
{
    bool pass = true;
    int val;
    HGLRC ctx;

    ctx = wglCreateContext(hdc);
    if (!ctx) {
        fprintf(stderr, "Failed to create wgl context\n");
        report_error(__FUNCTION__);
        return 1;
    }
    if (!wglMakeCurrent(hdc, ctx)) {
        fprintf(stderr, "Failed to make context current\n");
        return 1;
    }

    /* GL 1.0 APIs are available as symbols in opengl32.dll. */
    glEnable(GL_LIGHTING);
    val = 0;
    glGetIntegerv(GL_LIGHTING, &val);
    if (!val) {
        fprintf(stderr, "Enabling GL_LIGHTING didn't stick.\n");
        pass = false;
    }

    if (glproxy_gl_version() >= 15 ||
        glproxy_has_gl_extension("GL_ARB_vertex_buffer_object")) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 1234);

        val = 0;
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &val);
        if (val != 1234) {
            printf("GL_ELEMENT_ARRAY_BUFFER_BINDING didn't stick: %d\n", val);
            pass = false;
        }
    }
    if (glproxy_gl_version() >= 30)
    {
        int i;
        int num_extensions;
        fprintf(stdout, "Testing for gl version >= 30\n");
        glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
        for (i = 0; i < num_extensions; ++i) {
            const char *gl_ext = (const char *)glGetStringi(GL_EXTENSIONS, i);
            if (memcmp(gl_ext, "WGL_", 4) != 0) {
                pass = pass && glproxy_has_gl_extension(gl_ext);
                if (!pass) {
                    fprintf(stderr, "Can not found the extension:%s\n", gl_ext);
                }
            }
        }
    } else {
        const char* exts = glGetString(GL_EXTENSIONS);
        char* new_exts = strdup(exts);
        size_t pos = 0;
        size_t prev = 0;
        fprintf(stdout, "Testing for gl version < 30\n");
        fprintf(stdout, "The extensions are:%s\n", exts);
        while (true) {
            while (new_exts[pos] != ' ' && new_exts[pos] != '\0') {
                ++pos;
            }
            if (new_exts[pos] == '\0') {
                break;
            }
            new_exts[pos] = '\0';
            if (pos > prev) {
                const char *gl_ext = new_exts + prev;
                pass = pass && glproxy_has_gl_extension(gl_ext);
                if (!pass) {
                    fprintf(stderr, "Can not found the extension:%s\n", gl_ext);
                }
            }
            ++pos;
            prev = pos;
        }
        free(new_exts);
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(ctx);

    return !pass;
}

int main()
{
    make_window_and_test(test_function);

    /* UNREACHED */
    return 1;
}