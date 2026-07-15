# Android Integration Guide for SBA-Circuit-Forge

This document outlines the critical steps required to integrate the `circuit_server` C++ backend and the web-based UI assets into an Android Studio project. This guide assumes you have already set up your Android Studio project with NDK support and copied the necessary C++ and web assets into `app/src/main/cpp` and `app/src/main/assets/web` respectively.

## 1. Project Structure Verification (in Android Studio)

Ensure your Android Studio project is set up as follows:

```
AndroidStudioProject/
├── app/
│   ├── build.gradle
│   ├── src/
│   │   ├── main/
│   │   │   ├── AndroidManifest.xml
│   │   │   ├── java/                (Kotlin/Java source code for Android app)
│   │   │   │   └── com/yourcompany/circuitforge/
│   │   │   │       └── MainActivity.kt (or .java)
│   │   │   ├── res/                  (Android resources: layouts, drawables, etc.)
│   │   │   ├── assets/               (Web frontend files: HTML, CSS, JS, images)
│   │   │   │   └── web/
│   │   │   │       ├── index.html
│   │   │   │       ├── style.css
│   │   │   │       └── canvas_controller.js
│   │   │   └── cpp/                  (Native C++ source files, including engine_pulse)
│   │   │       ├── CMakeLists.txt    (Your top-level NDK CMakeLists.txt)
│   │   │       ├── engine_pulse/
│   │   │       │   ├── CMakeLists.txt
│   │   │       │   ├── PulseEngine.h
│   │   │       │   ├── PulseEngine.cpp
│   │   │       │   ├── generator.cpp
│   │   │       │   ├── server/
│   │   │       │   │   ├── main.cpp
│   │   │       │   │   └── Crow/ (Crow single-header or entire Crow source)
│   │   │       │   └── ...
│   │   │       ├── shared_schemas/
│   │   │       │   └── circuit.proto
│   │   │       └── ...
```

## 2. Android Manifest Permissions

Your Android application needs specific permissions to communicate with the local HTTP server. Add the following to your `app/src/main/AndroidManifest.xml` inside the `<manifest>` tag, usually before the `<application>` tag:

```xml
<uses-permission android:name="android.permission.INTERNET" />
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
```

## 3. Starting the `circuit_server` in a Background Service

To keep your `circuit_server` binary running in the background and managed by the Android system, it should be launched from an Android `Service`.

### Create a Service Class (e.g., `CircuitServerService.java` or `CircuitServerService.kt`)

```java
// app/src/main/java/com/yourcompany/circuitforge/CircuitServerService.java
package com.yourcompany.circuitforge;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Intent;
import android.os.Build;
import android.os.IBinder;
import android.util.Log;

import androidx.annotation.Nullable;
import androidx.core.app.NotificationCompat;

import java.io.IOException;

public class CircuitServerService extends Service {

    private static final String TAG = "CircuitServerService";
    private static final String CHANNEL_ID = "CircuitForgeChannel";
    private Process serverProcess;

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(TAG, "Service onCreate");
        createNotificationChannel();
        Notification notification = new NotificationCompat.Builder(this, CHANNEL_ID)
                .setContentTitle("Circuit Forge")
                .setContentText("Running circuit simulation backend")
                .setSmallIcon(android.R.drawable.ic_notification_overlay) // Replace with your app icon
                .build();
        startForeground(1, notification); // Start as foreground service
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d(TAG, "Service onStartCommand");

        // Start the native server in a new thread to prevent ANR
        new Thread(() -> {
            try {
                // Ensure your native library is loaded. This might be handled by System.loadLibrary("your_ndk_module_name")
                // in your main Application class or directly here if needed.
                // However, Runtime.getRuntime().exec will directly execute the binary you specify.

                // IMPORTANT: The path below assumes 'circuit_server' is bundled within your native library folder
                // (e.g., lib/arm64-v8a/circuit_server) and is executable.
                // You might need to adjust the path or copy the executable to a writable location first.
                // For simplicity, we assume it's directly executable from its installed NDK path.
                
                // Get the application's data directory where native libraries are installed
                String nativeLibraryDir = getApplicationInfo().nativeLibraryDir;
                String serverExecutablePath = nativeLibraryDir + "/circuit_server"; // Assuming it's installed as 'circuit_server'

                // Make the executable runnable (might be necessary on some Android versions/devices)
                Runtime.getRuntime().exec("chmod 755 " + serverExecutablePath).waitFor();

                Log.d(TAG, "Starting circuit_server from: " + serverExecutablePath);
                serverProcess = Runtime.getRuntime().exec(serverExecutablePath);

                // Optionally, read server output for debugging
                // BufferedReader reader = new BufferedReader(new InputStreamReader(serverProcess.getInputStream()));
                // String line;
                // while ((line = reader.readLine()) != null) {
                //     Log.d(TAG, "Server Output: " + line);
                // }

                serverProcess.waitFor(); // Wait for the process to exit (it shouldn't, unless stopped)
                Log.d(TAG, "Circuit server process finished.");

            } catch (IOException | InterruptedException e) {
                Log.e(TAG, "Error starting or running circuit server: " + e.getMessage());
                // Handle error, e.g., stop service, show notification
            } finally {
                stopSelf(); // Stop the service if the server process exits unexpectedly
            }
        }).start();

        return START_STICKY; // Service will be restarted if killed by the system
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "Service onDestroy");
        if (serverProcess != null) {
            serverProcess.destroy(); // Terminate the server process
            Log.d(TAG, "Circuit server process destroyed.");
        }
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null; // This service does not provide binding
    }

    private void createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            NotificationChannel serviceChannel = new NotificationChannel(
                    CHANNEL_ID,
                    "Circuit Forge Service Channel",
                    NotificationManager.IMPORTANCE_DEFAULT
            );
            NotificationManager manager = getSystemService(NotificationManager.class);
            if (manager != null) {
                manager.createNotificationChannel(serviceChannel);
            }
        }
    }
}
```

### Register the Service in `AndroidManifest.xml`

Add the service declaration inside the `<application>` tag in `app/src/main/AndroidManifest.xml`:

```xml
<service
    android:name=".CircuitServerService"
    android:enabled="true"
    android:exported="false"
    android:foregroundServiceType="dataSync" /> <!-- Required for Android 10+ for foreground services -->
```

### Start the Service from your `MainActivity`

From your `MainActivity` (e.g., in `onCreate` or when the user initiates it), start the service:

```java
// app/src/main/java/com/yourcompany/circuitforge/MainActivity.java
package com.yourcompany.circuitforge;

import android.content.Intent;
import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // setContentView(R.layout.activity_main); // Your layout

        // Start the background service for the C++ server
        Intent serviceIntent = new Intent(this, CircuitServerService.class);
        startService(serviceIntent);

        // ... rest of your activity setup, including WebView ...
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // Optionally stop the service when the activity is destroyed,
        // though START_STICKY might keep it running. Consider your app's lifecycle.
        // Intent serviceIntent = new Intent(this, CircuitServerService.class);
        // stopService(serviceIntent);
    }
}
```

## 4. WebView Configuration and Communication

In your `MainActivity` (or a Fragment/View where your WebView resides), configure the `WebView` to load your local web assets and communicate with the `circuit_server`.

```java
// app/src/main/java/com/yourcompany/circuitforge/MainActivity.java (continued)
package com.yourcompany.circuitforge;

import android.os.Bundle;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent; // Make sure this is imported

public class MainActivity extends AppCompatActivity {

    private WebView webView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main); // Assuming you have an activity_main.xml with a WebView

        // Start the background service for the C++ server
        Intent serviceIntent = new Intent(this, CircuitServerService.class);
        startService(serviceIntent);

        webView = findViewById(R.id.webView); // Assuming your layout has a WebView with id "webView"
        setupWebView();
        // Load your local HTML file from assets
        // The Crow server starts on port 18080 (127.0.0.1:18080), which your JS will fetch from.
        // The initial page loading from assets will also use file://
        webView.loadUrl("file:///android_asset/web/index.html");
    }

    private void setupWebView() {
        WebSettings webSettings = webView.getSettings();
        webSettings.setJavaScriptEnabled(true); // Enable JavaScript in the WebView
        webSettings.setDomStorageEnabled(true); // Enable DOM storage (e.g., localStorage)
        webSettings.setAllowFileAccess(true);   // Allow access to file:// URLs
        webSettings.setAllowContentAccess(true); // Allow content access

        // IMPORTANT: For debugging, you can enable WebView inspection in Chrome DevTools
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            WebView.setWebContentsDebuggingEnabled(true);
        }

        // Optional: Handle page navigation within the WebView itself
        webView.setWebViewClient(new WebViewClient() {
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                view.loadUrl(url);
                return true;
            }
        });
    }

    // ... other activity lifecycle methods ...
}
```

### Example `activity_main.xml` layout

```xml
<!-- app/src/main/res/layout/activity_main.xml -->
<?xml version="1.0" encoding="utf-8"?>
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    tools:context=".MainActivity">

    <WebView
        android:id="@+id/webView"
        android:layout_width="match_parent"
        android:layout_height="match_parent" />

</RelativeLayout>
```

## 5. NDK Build Configuration (Review `app/build.gradle`)

Ensure your `app/build.gradle` is configured to correctly build your C++ code using CMake. When you open your project in Android Studio and add a new C++ project (or import an existing CMakeLists.txt), Android Studio typically configures this for you.

```gradle
// app/build.gradle
android {
    // ... other configurations ...

    defaultConfig {
        // ...
        externalNativeBuild {
            cmake {
                // Pass the BUILD_FOR_ANDROID flag to your top-level CMakeLists.txt
                arguments "-DBUILD_FOR_ANDROID=ON"
                // Add any other NDK specific arguments if needed, e.g., for toolchain flags
                // cppFlags "-std=c++17 -Wall -Wextra"
            }
        }
    }

    buildTypes {
        release {
            // ...
        }
        debug {
            // ...
        }
    }

    // Specify the path to your top-level CMakeLists.txt
    externalNativeBuild {
        cmake {
            path file('src/main/cpp/CMakeLists.txt')
            version "3.18.1" // Or your preferred CMake version
        }
    }

    sourceSets {
        main {
            // This tells Android Studio where your web assets are located
            assets.srcDirs = ['src/main/assets/web']
        }
    }
}
```

## Next Steps in Android Studio

1.  **Create your Android Studio project**: If you haven't already, create a new Native C++ project in Android Studio, or open an existing one.
2.  **Copy Files**: Manually copy the `engine_pulse`, `shared_schemas`, and `app/src/main/assets/web` directories to the appropriate locations (`app/src/main/cpp` and `app/src/main/assets` respectively).
3.  **Update `AndroidManifest.xml`**: Add the permissions and service declaration.
4.  **Create `CircuitServerService.java/kt`**: Implement the service logic.
5.  **Modify `MainActivity.java/kt`**: Start the service and configure the WebView.
6.  **Create `activity_main.xml`**: Add a `WebView` component.
7.  **Review `app/build.gradle`**: Ensure CMake and asset paths are correctly configured.
8.  **Build and Run**: Build your Android application in Android Studio and run it on an emulator or device.

This README provides a comprehensive guide for bridging your C++ backend with your Android WebView frontend.
