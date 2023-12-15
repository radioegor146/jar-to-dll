import java.io.File;
import java.io.PrintWriter;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.security.ProtectionDomain;
import java.util.ArrayList;

public class ForgeInjector extends Thread {
    private byte[][] classes;

    private ForgeInjector(byte[][] classes) {
        this.classes = classes;
    }

    public static void inject(byte[][] classes) {
        new Thread(new ForgeInjector(classes)).start();
    }

    private static Class tryGetClass(PrintWriter writer, ClassLoader cl, String... names) throws ClassNotFoundException {
    	ClassNotFoundException lastException = null;
    	for (String name : names) {
    		try {
    			return cl.loadClass(name);
    		} catch (ClassNotFoundException e) {
    			lastException = e;
    		}
    	}
    	throw lastException;
    }

    @Override
    public void run() {
        try (PrintWriter writer = new PrintWriter(System.getProperty("user.home") + File.separator + "jar-to-dll-log.txt", "UTF-8")) {
            writer.println("Starting!");
            writer.flush();
            try {
                ClassLoader cl = null;
                for (Thread thread : Thread.getAllStackTraces().keySet()) {
                    ClassLoader threadLoader;
                    if (thread == null || thread.getContextClassLoader() == null || (threadLoader = thread.getContextClassLoader()).getClass() == null || 
                        threadLoader.getClass().getName() == null) continue;
                    String loaderName = threadLoader.getClass().getName();
                    writer.println("Thread: " + thread.getName() + " [" + loaderName + "]");
                    writer.flush();
                    if (!loaderName.contains("LaunchClassLoader") && !loaderName.contains("RelaunchClassLoader") && !loaderName.contains("TransformingClassLoader")) continue;
                    cl = threadLoader;
                    break;
                }
                if (cl == null) {
                    throw new Exception("ClassLoader is null");
                }
                this.setContextClassLoader(cl);
                Class modAnnotation = tryGetClass(writer, cl, "cpw.mods.fml.common.Mod", "net.minecraftforge.fml.common.Mod");
                Class forgeEventHandlerAnnotation = null;
                Class fmlInitializationEventClass = null;
                Class fmlPreInitializationEventClass = null;
                try {
                    forgeEventHandlerAnnotation = tryGetClass(writer, cl, "cpw.mods.fml.common.Mod$EventHandler", "net.minecraftforge.fml.common.Mod$EventHandler");
                    fmlPreInitializationEventClass = tryGetClass(writer, cl, "cpw.mods.fml.common.event.FMLPreInitializationEvent", "net.minecraftforge.fml.common.event.FMLPreInitializationEvent");
                    fmlInitializationEventClass = tryGetClass(writer, cl, "cpw.mods.fml.common.event.FMLInitializationEvent", "net.minecraftforge.fml.common.event.FMLInitializationEvent");
                } catch (Exception e) {
                    writer.println("Event handler annotations not found, probably new version of FML");
                }
                Method loadMethod = ClassLoader.class.getDeclaredMethod("defineClass", String.class, byte[].class, Integer.TYPE, Integer.TYPE, ProtectionDomain.class);
                loadMethod.setAccessible(true);
                writer.println("Loading " + classes.length + " classes");
                writer.flush();
                ArrayList<Object[]> mods = new ArrayList<>();
                for (byte[] classData : classes) {
                    if (classData == null) {
                        throw new Exception("classData is null");
                    }
                    if (cl.getClass() == null) {
                        throw new Exception("getClass() is null");
                    }
                    try {
                        Class tClass = null;
                        try {
                            tClass = (Class)loadMethod.invoke(cl, null, classData, 0, classData.length, cl.getClass().getProtectionDomain());
                        } catch (Throwable e) {
                            if (!(e instanceof LinkageError)) {
                                throw e;
                            }

                            if (e.getMessage().contains("duplicate class definition for name: ")) {
                                String className = e.getMessage().split("\"")[1];
                                tClass = cl.loadClass(className.replace('/', '.'));
                                writer.println("It is recommended to remove " + className + ".class from your input.jar");
                            }
                        }
                        if (tClass.getAnnotation(modAnnotation) == null) 
                        	continue;
                        Object[] mod = new Object[3];
                        mod[0] = tClass;
                        ArrayList<Method> fmlPreInitMethods = new ArrayList<Method>();
                        ArrayList<Method> fmlInitMethods = new ArrayList<Method>();
                        if (forgeEventHandlerAnnotation != null) {
                            for (Method m : tClass.getDeclaredMethods()) {
                                if (m.getAnnotation(forgeEventHandlerAnnotation) != null && m.getParameterCount() == 1 && m.getParameterTypes()[0] == fmlInitializationEventClass) {
                                    m.setAccessible(true);
                                    fmlInitMethods.add(m);
                                }
                                if (m.getAnnotation(forgeEventHandlerAnnotation) != null && m.getParameterCount() == 1 && m.getParameterTypes()[0] == fmlPreInitializationEventClass) {
                                    m.setAccessible(true);
                                    fmlPreInitMethods.add(m);
                                }
                            }
                        }
                        mod[1] = fmlPreInitMethods;
                        mod[2] = fmlInitMethods;
                        mods.add(mod);
                    }
                    catch (Exception e) {
                        e.printStackTrace();
                        throw new Exception("Exception on defineClass", e);
                    }
                }
                writer.println(classes.length + " loaded successfully");
                writer.flush();
                for (Object[] mod : mods) {
                	Class modClass = (Class) mod[0];
                	ArrayList<Method> fmlPreInitMethods = (ArrayList<Method>) mod[1];
                	ArrayList<Method> fmlInitMethods = (ArrayList<Method>) mod[2];
                	Object modInstance = null;

					try {
                        writer.println("Instancing " + modClass.getName());
                        writer.flush();
                        modInstance = modClass.newInstance();
                        writer.println("Instanced");
                        writer.flush();
                    }
                    catch (Exception e) {
                        writer.println("Genexeption on instancing: " + e);
                        e.printStackTrace(writer);
                        writer.flush();
                        throw new Exception("Exception on instancing", e);
                    }

                    for (Method preInitMethod : fmlPreInitMethods) {
	                    try {
	                        writer.println("Preiniting " + preInitMethod);
	                        writer.flush();
	                        writer.println("Preinited");
	                        writer.flush();
	                        preInitMethod.invoke(modInstance, new Object[]{null});
	                    }
	                    catch (InvocationTargetException e) {
	                        writer.println("InvocationTargetException on preiniting: " + e);
	                        e.getCause().printStackTrace(writer);
	                        writer.flush();
	                        throw new Exception("Exception on preiniting (InvocationTargetException)", e.getCause());
	                    }
	                    catch (Exception e) {
	                        writer.println("Genexeption on preiniting: " + e);
	                        e.printStackTrace(writer);
	                        writer.flush();
	                        throw new Exception("Exception on preiniting", e);
	                    }
                	}

                	for (Method initMethod : fmlInitMethods) {
	                    try {
	                        writer.println("Initing " + initMethod);
	                        writer.flush();
	                        writer.println("Inited");
	                        writer.flush();
	                        initMethod.invoke(modInstance, new Object[]{null});
	                    }
	                    catch (InvocationTargetException e) {
	                        writer.println("InvocationTargetException on initing: " + e);
	                        e.getCause().printStackTrace(writer);
	                        writer.flush();
	                        throw new Exception("Exception on initing (InvocationTargetException)", e.getCause());
	                    }
	                    catch (Exception e) {
	                        writer.println("Genexeption on initing: " + e);
	                        e.printStackTrace(writer);
	                        writer.flush();
	                        throw new Exception("Exception on initing", e);
	                    }
                	}
                }
                writer.println("Successfully injected");
                writer.flush();
            }
            catch (Throwable e) {
                e.printStackTrace(writer);
                writer.flush();
            }
            writer.close();
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
    }
}