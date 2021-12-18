#include <jni.h>
#include <string>

static jobject globalPerson;
static jobject globalWeakPerson;

extern "C" JNIEXPORT void JNICALL
Java_com_whf_opengldemo_MainActivity_sendPerson(
        JNIEnv* env,
        jobject MainActivity /* this */,
        jobject Person) {
    //globalPerson = env->NewGlobalRef(Person);
    globalWeakPerson = env->NewWeakGlobalRef(Person);
}


extern "C" JNIEXPORT jobject JNICALL
Java_com_whf_opengldemo_MainActivity_getPerson(
        JNIEnv* env,
        jobject MainActivity /* this */) {

    //jobject localPerson = env->NewLocalRef(globalPerson);
    //env->DeleteGlobalRef(globalPerson);
    if (env->IsSameObject(globalWeakPerson,NULL) == JNI_FALSE){
        return globalWeakPerson;
    }

    return nullptr;
}
