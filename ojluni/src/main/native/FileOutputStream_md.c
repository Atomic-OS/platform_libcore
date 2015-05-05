/*
 * Copyright (c) 1997, 2010, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

#include "jni.h"
#include "jni_util.h"
#include "jvm.h"

#include "io_util.h"
#include "io_util_md.h"
#include "java_io_FileOutputStream.h"

#include <fcntl.h>
#include "JNIHelp.h"

#define NATIVE_METHOD(className, functionName, signature) \
{ #functionName, signature, (void*)(className ## _ ## functionName) }

/*******************************************************************/
/*  BEGIN JNI ********* BEGIN JNI *********** BEGIN JNI ************/
/*******************************************************************/

jfieldID fos_fd; /* id for jobject 'fd' in java.io.FileOutputStream */

/**************************************************************
 * static methods to store field ID's in initializers
 */

JNIEXPORT void JNICALL
FileOutputStream_initIDs(JNIEnv *env, jclass fdClass) {
    fos_fd = (*env)->GetFieldID(env, fdClass, "fd", "Ljava/io/FileDescriptor;");
}

/**************************************************************
 * Output stream
 */

JNIEXPORT void JNICALL
FileOutputStream_open(JNIEnv *env, jobject this,
                                   jstring path, jboolean append) {
    fileOpen(env, this, path, fos_fd,
             O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC));
}

JNIEXPORT void JNICALL
FileOutputStream_write(JNIEnv *env, jobject this, jint byte, jboolean append) {
    writeSingle(env, this, byte, append, fos_fd);
}

JNIEXPORT void JNICALL
FileOutputStream_writeBytes(JNIEnv *env,
    jobject this, jbyteArray bytes, jint off, jint len, jboolean append) {
    writeBytes(env, this, bytes, off, len, append, fos_fd);
}

JNIEXPORT void JNICALL
FileOutputStream_close0(JNIEnv *env, jobject this) {
    fileClose(env, this, fos_fd);
}

static JNINativeMethod gMethods[] = {
  NATIVE_METHOD(FileOutputStream, initIDs, "()V"),
  NATIVE_METHOD(FileOutputStream, open, "(Ljava/lang/String;Z)V"),
  NATIVE_METHOD(FileOutputStream, write, "(IZ)V"),
  NATIVE_METHOD(FileOutputStream, writeBytes, "([BIIZ)V"),
  NATIVE_METHOD(FileOutputStream, close0, "()V"),
};

void register_java_io_FileOutputStream(JNIEnv* env) {
  jniRegisterNativeMethods(env, "java/io/FileOutputStream", gMethods, NELEM(gMethods));
}
