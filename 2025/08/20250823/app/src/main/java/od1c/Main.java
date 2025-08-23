package od1c;
import org.lwjgl.glfw.*;
import org.lwjgl.opengl.*;
import org.lwjgl.system.MemoryUtil;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.nio.FloatBuffer;
import java.util.Locale;
import java.util.Objects;
import java.util.concurrent.ConcurrentLinkedQueue;

import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL15.*;   // ★追加
import static org.lwjgl.opengl.GL20.*;
import static org.lwjgl.opengl.GL30.*;   // ★追加
import static org.lwjgl.system.MemoryUtil.NULL;


public class Main {

    // 共有状態
    private volatile float speedDegPerSec = 90f;           // 回転速度（度/秒）
    private volatile float colorR = 1f, colorG = 1f, colorB = 1f;
    private volatile Shape currentShape = Shape.TRIANGLE;  // triangle | quad | star
    private volatile boolean requestQuit = false;

    // GL/GLFW
    private long window;
    private int prog;          // シェーダプログラム
    private int vao, vbo;      // 図形用
    private int uAngleLoc, uColorLoc;
    private int vertexCount = 0;

    // 描画用
    private float angleRad = 0f;
    private double lastTime = 0.0;

    // コマンド受信用
    private final ConcurrentLinkedQueue<String> commandQueue = new ConcurrentLinkedQueue<>();

    enum Shape { TRIANGLE, QUAD, STAR }

    public static void main(String[] args) {
        Locale.setDefault(Locale.US); // 小数点は . を想定
        new Main().run();
    }

    private void run() {
        // 1) GLFW 初期化 & ウィンドウ作成
        if (!glfwInit()) throw new IllegalStateException("Failed to init GLFW");
        glfwSetErrorCallback((code, desc) -> System.err.println("GLFW ERR " + code + ": " + MemoryUtil.memUTF8(desc)));

        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        window = glfwCreateWindow(800, 600, "OD1C OpenGL", NULL, NULL);
        if (window == NULL) throw new RuntimeException("Failed to create window");

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // VSync ON
        glfwShowWindow(window);

        // フレームバッファサイズ変更で viewport 更新
        // glfwSetFramebufferSizeCallback(window, (w, h, width, height) -> glViewport(0, 0, width, height));
        glfwSetFramebufferSizeCallback(window, (win, width, height) -> glViewport(0, 0, width, height));

        // 2) OpenGL 機能テーブルを使用可能に
        GL.createCapabilities();

        // 3) シェーダを用意
        prog = createProgram(VERT, FRAG);
        glUseProgram(prog);
        uAngleLoc = glGetUniformLocation(prog, "uAngle");
        uColorLoc = glGetUniformLocation(prog, "uColor");

        // 4) 図形バッファ（VBO/VAO）作成（初期 TRIANGLE）
        int[] vaoArr = new int[1];
        vao = glGenVertexArrays();
        vbo = glGenBuffers();
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        updateShapeBuffer(currentShape);

        glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * Float.BYTES, 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        // 背景色
        glClearColor(0.1f, 0.1f, 0.12f, 1f);

        // 標準入力スレッド：1 行ずつキューへ
        startStdinThread();

        System.out.println("READY");
        lastTime = glfwGetTime();

        // 5) メインループ
        int frames = 0;
        double fpsTime = 0.0;
        while (!glfwWindowShouldClose(window) && !requestQuit) {
            glfwPollEvents();

            // コマンド処理（レンダースレッド側で反映）
            drainCommands();

            // デルタタイム
            double now = glfwGetTime();
            double dt = now - lastTime;
            lastTime = now;

            // 回転
            angleRad += Math.toRadians(speedDegPerSec) * (float) dt;

            // 描画
            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(prog);
            glUniform1f(uAngleLoc, angleRad);
            glUniform3f(uColorLoc, colorR, colorG, colorB);

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
            glBindVertexArray(0);

            glfwSwapBuffers(window);

            // FPS 計測（必要な時に使う）
            frames++;
            fpsTime += dt;
            if (fpsTime >= 1.0) {
                // ここで最新 FPS を更新しておく（fps コマンドで出す）
                latestFps = frames / fpsTime;
                frames = 0; fpsTime = 0.0;
            }
        }

        // 6) 後片付け
        System.out.println("BYE");
        glDeleteProgram(prog);
        glDeleteBuffers(vbo);
        glDeleteVertexArrays(vao);
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    // ==== コマンド処理 ====
    private volatile double latestFps = 0.0;

    private void startStdinThread() {
        Thread t = new Thread(() -> {
            try (BufferedReader br = new BufferedReader(new InputStreamReader(System.in))) {
                String line;
                while ((line = br.readLine()) != null) {
                    if (!line.isBlank()) commandQueue.add(line.trim());
                }
            } catch (Exception e) {
                // 標準入力が閉じられたら何もしない
            }
        }, "stdin-reader");
        t.setDaemon(true);
        t.start();
    }

    private void drainCommands() {
        String line;
        while ((line = commandQueue.poll()) != null) {
            handleCommand(line);
        }
    }

    private void handleCommand(String line) {
        String[] tok = line.trim().split("\\s+");
        String cmd = tok[0].toLowerCase(Locale.ROOT);

        try {
            switch (cmd) {
                case "speed": {
                    if (tok.length != 2) { err("usage: speed <float>"); return; }
                    speedDegPerSec = Float.parseFloat(tok[1]);
                    System.out.println("OK SPEED " + speedDegPerSec);
                    break;
                }
                case "color": {
                    if (tok.length != 4) { err("usage: color <r> <g> <b>"); return; }
                    colorR = Float.parseFloat(tok[1]);
                    colorG = Float.parseFloat(tok[2]);
                    colorB = Float.parseFloat(tok[3]);
                    System.out.printf(Locale.US, "OK COLOR %.3f %.3f %.3f%n", colorR, colorG, colorB);
                    break;
                }
                case "shape": {
                    if (tok.length != 2) { err("usage: shape <triangle|quad|star>"); return; }
                    Shape s = switch (tok[1].toLowerCase(Locale.ROOT)) {
                        case "triangle" -> Shape.TRIANGLE;
                        case "quad" -> Shape.QUAD;
                        case "star" -> Shape.STAR;
                        default -> null;
                    };
                    if (s == null) { err("unknown shape"); return; }
                    if (s != currentShape) {
                        currentShape = s;
                        updateShapeBuffer(currentShape);
                    }
                    System.out.println("OK SHAPE " + tok[1].toLowerCase(Locale.ROOT));
                    break;
                }
                case "vsync": {
                    if (tok.length != 2) { err("usage: vsync <on|off>"); return; }
                    boolean on = Objects.equals(tok[1].toLowerCase(Locale.ROOT), "on");
                    glfwSwapInterval(on ? 1 : 0);
                    System.out.println("OK VSYNC " + (on ? "on" : "off"));
                    break;
                }
                case "fps": {
                    System.out.printf(Locale.US, "FPS %.1f%n", latestFps);
                    break;
                }
                case "quit": {
                    requestQuit = true;
                    break;
                }
                default:
                    err("unknown command");
            }
        } catch (Exception ex) {
            err("bad args");
        }
    }

    private void err(String msg) { System.out.println("ERR " + msg); }

    private void updateShapeBuffer(Shape s) {
        float[] verts;
        switch (s) {
            case TRIANGLE -> verts = vertsTriangle();
            case QUAD     -> verts = vertsQuad();
            case STAR     -> verts = vertsStarFan(); // 中心からの三角形ファン
            default -> throw new IllegalStateException("unexpected");
        }
        vertexCount = verts.length / 2;

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // FloatBuffer にして転送
        FloatBuffer fb = MemoryUtil.memAllocFloat(verts.length);
        fb.put(verts).flip();
        glBufferData(GL_ARRAY_BUFFER, fb, GL_STATIC_DRAW);
        MemoryUtil.memFree(fb);
    }

    private float[] vertsTriangle() {
        // 画面一杯に見える正三角形（原点中心、回転しやすいスケール）
        return new float[]{
                0.0f,  0.8f,
               -0.7f, -0.6f,
                0.7f, -0.6f
        };
    }

    private float[] vertsQuad() {
        // 2 三角形で四角形
        return new float[]{
                -0.7f, -0.7f,   0.7f, -0.7f,   0.7f,  0.7f,
                -0.7f, -0.7f,   0.7f,  0.7f,  -0.7f,  0.7f
        };
    }

    private float[] vertsStarFan() {
        // 五芒星の輪郭点（外半径 R=0.85、内半径 r=0.35）を中心(0,0)からの三角形ファンで塗る
        int spikes = 5;
        double R = 0.85, r = 0.35;
        float[] ring = new float[spikes * 2 * 2]; // (外,内)×5 個の xy
        int idx = 0;
        for (int i = 0; i < spikes; i++) {
            double tOuter = Math.toRadians(90 + i * 72);      // 上向きから開始
            double tInner = Math.toRadians(90 + i * 72 + 36); // 間の角度
            ring[idx++] = (float)(R * Math.cos(tOuter));
            ring[idx++] = (float)(R * Math.sin(tOuter));
            ring[idx++] = (float)(r * Math.cos(tInner));
            ring[idx++] = (float)(r * Math.sin(tInner));
        }
        // 三角形ファン：中心→点i→点i+1
        float[] verts = new float[(ring.length) * 3]; // 10 辺 → 10 三角形 → 頂点 30 → xy 60
        int vi = 0;
        for (int i = 0; i < ring.length; i += 2) {
            int j = (i + 2) % ring.length;
            // center
            verts[vi++] = 0f; verts[vi++] = 0f;
            // ring[i]
            verts[vi++] = ring[i];     verts[vi++] = ring[i + 1];
            // ring[j]
            verts[vi++] = ring[j];     verts[vi++] = ring[j + 1];
        }
        return verts;
    }

    // 頂点/フラグメント・シェーダ（最小）
    private static final String VERT = """
            #version 330 core
            layout (location = 0) in vec2 aPos;
            uniform float uAngle;
            void main(){
                float c = cos(uAngle), s = sin(uAngle);
                vec2 p = vec2(c*aPos.x - s*aPos.y, s*aPos.x + c*aPos.y);
                gl_Position = vec4(p, 0.0, 1.0);
            }
            """;

    private static final String FRAG = """
            #version 330 core
            out vec4 FragColor;
            uniform vec3 uColor;
            void main(){
                FragColor = vec4(uColor, 1.0);
            }
            """;

    private static int createProgram(String vs, String fs) {
        int v = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(v, vs);
        glCompileShader(v);
        if (glGetShaderi(v, GL_COMPILE_STATUS) == GL_FALSE)
            throw new RuntimeException("VS compile error: " + glGetShaderInfoLog(v));

        int f = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(f, fs);
        glCompileShader(f);
        if (glGetShaderi(f, GL_COMPILE_STATUS) == GL_FALSE)
            throw new RuntimeException("FS compile error: " + glGetShaderInfoLog(f));

        int p = glCreateProgram();
        glAttachShader(p, v);
        glAttachShader(p, f);
        glLinkProgram(p);
        if (glGetProgrami(p, GL_LINK_STATUS) == GL_FALSE)
            throw new RuntimeException("Program link error: " + glGetProgramInfoLog(p));

        glDeleteShader(v);
        glDeleteShader(f);
        return p;
    }
}
