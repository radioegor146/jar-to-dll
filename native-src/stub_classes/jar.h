#ifndef STUB_CLASSES_JAR_H_
#define STUB_CLASSES_JAR_H_

#include "../jvm/jni.h"

const jbyte test_class_1[] = {0x01, 0x02, 0x03};

const jbyte* jar_classes_data[] = {test_class_1};
const jsize jar_classes_sizes[] = {3};

#endif  //STUB_CLASSES_JAR_H_
