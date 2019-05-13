#include "main.h"
#include "jni.h"
#include "jvmti.h"
#include "classes.h"
#include "loader.h"

VOID OutputLastError(DWORD errorCode)
{
    CHAR errorString[256] = {0};
    sprintf(errorString, "Last error code: %lu", errorCode);
    MessageBox(NULL, errorString, "ELoader", MB_OK | MB_ICONEXCLAMATION);
}

DWORD WINAPI MainThread(CONST LPVOID lpParam)
{
    MessageBoxA(NULL, "Starting injecting", "ELoader", MB_OK | MB_ICONINFORMATION);
    HMODULE jvmDll = GetModuleHandleA("jvm.dll");
    if (!jvmDll)
    {
        DWORD lastError = GetLastError();
        MessageBoxA(NULL, "Can't find jvm.dll module handle", "ELoader", MB_OK | MB_ICONERROR);
        OutputLastError(lastError);
        ExitThread(0);
    }
    FARPROC getJvmsVoidPtr = GetProcAddress(jvmDll, "JNI_GetCreatedJavaVMs");
    if (!getJvmsVoidPtr)
    {
        DWORD lastError = GetLastError();
        MessageBoxA(NULL, "Can't find JNI_GetCreatedJavaVMs proc handle", "ELoader", MB_OK | MB_ICONERROR);
        OutputLastError(lastError);
        ExitThread(0);
    }


    ///       DEHOOKING
    if (MessageBox(NULL, "Should I do dehooking? (not recommended with avanguard protection)", "ELoader", MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        CHAR jvmDllPath[MAX_PATH] = {0};
        if (!GetModuleFileNameA(jvmDll, jvmDllPath, _countof(jvmDllPath)))
        {
            DWORD lastError = GetLastError();
            MessageBoxA(NULL, "Dehooking failed - 'GetModuleFileNameA' returned null", "ELoader", MB_OK | MB_ICONERROR);
            OutputLastError(lastError);
            ExitThread(0);
        }
        CHAR czTempPath[MAX_PATH] = {0};
        if (!GetTempPathA(MAX_PATH, czTempPath))
        {
            DWORD lastError = GetLastError();
            MessageBoxA(NULL, "Dehooking failed - 'GetTempPathA' returned null", "ELoader", MB_OK | MB_ICONERROR);
            OutputLastError(lastError);
            ExitThread(0);
        }
        CHAR jvmDllTempPath[MAX_PATH] = {0};
        if (!GetTempFileNameA(czTempPath, "jvm", 0, jvmDllTempPath))
        {
            DWORD lastError = GetLastError();
            MessageBoxA(NULL, "Dehooking failed - 'GetTempFileNameA' returned null", "ELoader", MB_OK | MB_ICONERROR);
            OutputLastError(lastError);
            ExitThread(0);
        }
        if (!CopyFile(jvmDllPath, jvmDllTempPath, FALSE))
        {
            DWORD lastError = GetLastError();
            MessageBoxA(NULL, "Dehooking failed - 'CopyFile' returned null", "ELoader", MB_OK | MB_ICONERROR);
            OutputLastError(lastError);
            ExitThread(0);
        }
        HMODULE tempJvmDll = LoadLibraryA(jvmDllTempPath);
        if (!tempJvmDll)
        {
            DWORD lastError = GetLastError();
            MessageBoxA(NULL, "Dehooking failed - 'LoadLibraryA' returned null", "ELoader", MB_OK | MB_ICONERROR);
            OutputLastError(lastError);
            ExitThread(0);
        }
        FARPROC getJvmsExtVoidPtr = GetProcAddress(tempJvmDll, "JNI_GetCreatedJavaVMs");
        if (!getJvmsExtVoidPtr)
        {
            DWORD lastError = GetLastError();
            MessageBoxA(NULL, "Dehooking failed - Can't find JNI_GetCreatedJavaVMs proc handle from temp dll", "ELoader", MB_OK | MB_ICONERROR);
            OutputLastError(lastError);
            ExitThread(0);
        }
        HANDLE currentProcess = GetCurrentProcess();
        if (!currentProcess)
        {
            DWORD lastError = GetLastError();
            MessageBoxA(NULL, "Dehooking failed - 'GetCurrentProcess' returned null", "ELoader", MB_OK | MB_ICONERROR);
            OutputLastError(lastError);
            ExitThread(0);
        }
        DWORD oldProtect = 0;
        if (!VirtualProtectEx(currentProcess, (LPVOID)getJvmsVoidPtr, 32, PAGE_EXECUTE_READWRITE, &oldProtect))
        {
            DWORD lastError = GetLastError();
            MessageBoxA(NULL, "Dehooking failed - 'VirtualProtectEx' returned null", "ELoader", MB_OK | MB_ICONERROR);
            OutputLastError(lastError);
            ExitThread(0);
        }
        memcpy((LPVOID)getJvmsVoidPtr, (LPVOID)getJvmsExtVoidPtr, 32);
        DWORD tProtect = 0;
        if (!VirtualProtectEx(currentProcess, (LPVOID)getJvmsVoidPtr, 32, oldProtect, &tProtect))
        {
            DWORD lastError = GetLastError();
            MessageBoxA(NULL, "Dehooking failed - 'VirtualProtectEx' 2 returned null", "ELoader", MB_OK | MB_ICONERROR);
            OutputLastError(lastError);
            ExitThread(0);
        }
        if (!FreeLibrary(tempJvmDll))
        {
            DWORD lastError = GetLastError();
            MessageBoxA(NULL, "Dehooking failed - 'FreeLibrary' returned null", "ELoader", MB_OK | MB_ICONERROR);
            OutputLastError(lastError);
            ExitThread(0);
        }
        if (!DeleteFileA(jvmDllTempPath))
        {
            DWORD lastError = GetLastError();
            MessageBoxA(NULL, "Dehooking failed - 'DeleteFileA' returned null", "ELoader", MB_OK | MB_ICONERROR);
            OutputLastError(lastError);
            ExitThread(0);
        }
        MessageBoxA(NULL, "GetCreatedJavaVMs has been successfully dehooked", "ELoader", MB_OK | MB_ICONINFORMATION);
    }
    ///---------------------


    typedef jint (JNICALL * GetCreatedJavaVMs)(JavaVM **, jsize, jsize *);
    GetCreatedJavaVMs jni_GetCreatedJavaVMs = (GetCreatedJavaVMs)getJvmsVoidPtr;
    jsize nVMs;
	jni_GetCreatedJavaVMs(NULL, 0, &nVMs);
	JavaVM **buffer = new JavaVM *[nVMs];
	jni_GetCreatedJavaVMs(buffer, nVMs, &nVMs);
	if (nVMs == 0)
    {
        MessageBoxA(NULL, "JVM not found!", "ELoader", MB_OK | MB_ICONERROR);
        ExitThread(0);
	}
    if (nVMs > 0)
    {
        for (jsize i = 0; i < nVMs; i++)
        {
            JavaVM* jvm = buffer[i];
			JNIEnv* jniEnv = NULL;
			jvmtiEnv* jvmTiEnv = NULL;
            jvm->AttachCurrentThread((void **)(&jniEnv), 0);
            jvm->GetEnv((void **)(&jniEnv), JNI_VERSION_1_8);
            if (!jniEnv)
            {
                MessageBoxA(NULL, "Can't attach to JNIEnv", "ELoader", MB_OK | MB_ICONERROR);
                jvm->DetachCurrentThread();
                break;
            }
            jclass classLoaderClazz = NULL;
            if (MessageBox(NULL, "Should I do megadefine (beta feature) (by Left4Dead)?", "ELoader", MB_YESNO | MB_ICONQUESTION) == IDYES)
            {
                jvm->AttachCurrentThread((void **)(&jvmTiEnv), 0);
                jvm->GetEnv((void **)(&jvmTiEnv), JVMTI_VERSION);
                if (!jvmTiEnv)
                {
                    MessageBoxA(NULL, "Can't attach to JVMTI", "ELoader", MB_OK | MB_ICONERROR);
                    jvm->DetachCurrentThread();
                    break;
                }
                jclass *loadedClasses;
                jint loadedClassesCount = 0;
                jvmTiEnv->GetLoadedClasses(&loadedClassesCount, &loadedClasses);
                jclass launchHandlerClass = NULL;
                jclass launchClassLoaderClass = NULL;
                for (jint i = 0; i < loadedClassesCount; i++)
                {
                    char *signature;
                    jvmTiEnv->GetClassSignature(loadedClasses[i], &signature, NULL);
                    if (!strcmp(signature, "Lcpw/mods/fml/relauncher/FMLLaunchHandler;"))
                    {
                        launchHandlerClass = loadedClasses[i];
                    }
                    if (!strcmp(signature, "Lnet/minecraft/launchwrapper/LaunchClassLoader;"))
                    {
                        launchClassLoaderClass = loadedClasses[i];
                    }
                    jvmTiEnv->Deallocate((unsigned char *)signature);
                }
                if (!launchHandlerClass || !launchClassLoaderClass)
                {
                    MessageBoxA(NULL, "Can't find FMLLaunchHandler or LaunchClassLoader", "ELoader", MB_OK | MB_ICONERROR);
                    jvm->DetachCurrentThread();
                    break;
                }
                else
                {
                    jfieldID instanceLaunchHandlerField = jniEnv->GetStaticFieldID(launchHandlerClass, "INSTANCE", "Lcpw/mods/fml/relauncher/FMLLaunchHandler;");
                    if (!instanceLaunchHandlerField)
                    {
                        MessageBoxA(NULL, "Can't get field INSTANCE of FMLLaunchHandler", "ELoader", MB_OK | MB_ICONERROR);
                        jvm->DetachCurrentThread();
                        break;
                    }
                    jobject launchHandler = jniEnv->GetStaticObjectField(launchHandlerClass, instanceLaunchHandlerField);
                    if (!launchHandler)
                    {
                        MessageBoxA(NULL, "Can't get INSTANCE of FMLLaunchHandler", "ELoader", MB_OK | MB_ICONERROR);
                        jvm->DetachCurrentThread();
                        break;
                    }
                    jfieldID classLoaderField = jniEnv->GetFieldID(launchHandlerClass, "classLoader", "Lnet/minecraft/launchwrapper/LaunchClassLoader;");
                    if (!classLoaderField)
                    {
                        MessageBoxA(NULL, "Can't get field classLoader of LaunchClassLoader", "ELoader", MB_OK | MB_ICONERROR);
                        jvm->DetachCurrentThread();
                        break;
                    }
                    jobject classLoader = jniEnv->GetObjectField(launchHandler, classLoaderField);
                    if (!classLoader)
                    {
                        MessageBoxA(NULL, "Can't get classLoader of LaunchClassLoader", "ELoader", MB_OK | MB_ICONERROR);
                        jvm->DetachCurrentThread();
                        break;
                    }
                    jfieldID defaultProtectionDomainField = jniEnv->GetFieldID(launchClassLoaderClass, "defaultDomain", "Ljava/security/ProtectionDomain;");
                    if (!classLoader)
                    {
                        MessageBoxA(NULL, "Can't get field defaultProtectionDomainField of LaunchClassLoader", "ELoader", MB_OK | MB_ICONERROR);
                        jvm->DetachCurrentThread();
                        break;
                    }
                    jobject protectionDomain = jniEnv->GetObjectField(classLoader, defaultProtectionDomainField);
                    if (!protectionDomain)
                    {
                        MessageBoxA(NULL, "Can't get protectionDomain of LaunchClassLoader", "ELoader", MB_OK | MB_ICONERROR);
                        jvm->DetachCurrentThread();
                        break;
                    }
                    FARPROC jvmDefineClassWithSource = GetProcAddress(jvmDll, "JVM_DefineClassWithSource");
                    if (!jvmDefineClassWithSource)
                    {
                        MessageBoxA(NULL, "Can't find JVM_DefineClassWithSource proc handle", "ELoader", MB_OK | MB_ICONERROR);
                        jvm->DetachCurrentThread();
                        break;
                    }
                    typedef jclass (JNICALL * DefineClassWithSource)(JNIEnv *, const char *, jobject, const jbyte *, jsize, jobject, const char *);
                    DefineClassWithSource jvm_DefineClassWithSource = (DefineClassWithSource)jvmDefineClassWithSource;
                    classLoaderClazz = jvm_DefineClassWithSource(jniEnv, "magictheinjecting/MagicTheInjecting", classLoader, (jbyte*)classLoaderClass, classLoaderClassSize, protectionDomain, "radioegor146");
                }
            }
            else
            {
                classLoaderClazz = jniEnv->DefineClass(NULL, NULL, (jbyte*)classLoaderClass, classLoaderClassSize);
            }
            if (!classLoaderClazz)
            {
                MessageBoxA(NULL, "Error on class defining", "ELoader", MB_OK | MB_ICONERROR);
                jvm->DetachCurrentThread();
                break;
            }
            jobjectArray classesData = (jobjectArray)jniEnv->CallStaticObjectMethod(classLoaderClazz, jniEnv->GetStaticMethodID(classLoaderClazz, "getByteArray", "(I)[[B"), (jint)classCount);//jniEnv->NewObjectArray(classCount, jniEnv->FindClass("[B"), NULL);
            int cptr = 0;
            for (jsize j = 0; j < classCount; j++)
            {
                jbyteArray classByteArray = jniEnv->NewByteArray(classSizes[j]);
                jniEnv->SetByteArrayRegion(classByteArray, 0, classSizes[j], (jbyte *)(classes + cptr));
                cptr += classSizes[j];
                jniEnv->SetObjectArrayElement(classesData, j, classByteArray);
            }
            jint injectResult = jniEnv->CallStaticIntMethod(classLoaderClazz, jniEnv->GetStaticMethodID(classLoaderClazz, "injectCP", "([[B)I"), classesData);
            if (injectResult)
            {
                MessageBoxA(NULL, "Error on injecting: injectResult != 0", "ELoader", MB_OK | MB_ICONERROR);
                jvm->DetachCurrentThread();
                break;
            }
            MessageBoxA(NULL, "Injected successfully!", "ELoader", MB_OK | MB_ICONINFORMATION);
            jvm->DetachCurrentThread();
        }
    }
    ExitThread(0);
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            CreateThread(NULL, 0, &MainThread, NULL, 0, NULL);
            break;
    }
    return TRUE; // succesful
}
