# Game Development in C and C++
## A project-based roadmap: 2D → performance → graphics → C++ → 3D → ship → specialize

Approximately **26 months at 10–15 h/week**, or 13–15 months at 25+ h/week. Nine projects, three of which are complete games you finish and release.

This merges two earlier drafts. The sequencing, scope discipline, and specialization branching come from the revision, which was right that shipping a real game early matters more than topic coverage. Five things the revision cut are restored, because they're cheap to learn and disproportionately useful: hot code reloading, generational handles, pathfinding, serialization with reference fixup, and open-source contribution.

**The governing principle:**

> Learn a system because a game needs it, not because a curriculum lists it.

---

# 1. What this optimizes for

A programmer who wants to be genuinely strong in C and C++, understand what sits underneath an engine, build performance-conscious systems, and finish games rather than accumulate technology demos. At the end you can move between gameplay, data structures, memory, the OS, the renderer, and the GPU without an engine hiding the interesting parts, and you have the option of engine, graphics, gameplay, tools, or indie work.

It does not attempt to teach every corner of C++, every rendering technique, or every engine. Depth comes from a small number of serious projects.

---

# 2. Operating rules

These matter more than the project list.

**1. Finish games.** A finished small game beats an impressive unfinished engine. Finished means: builds from a clean clone, has an entry point and a complete loop, a win or loss condition, restart, clean exit, a README, a release build, and a written postmortem.

**2. No engine before you've earned it.** Do not open with `EntityManager`, `ResourceSystem`, `EventBus`, or an ECS. Build a game, hit a problem, solve it, hit it again, *then* extract the abstraction. Every reusable system in this plan is extracted after you needed it twice.

**3. C first, not C forever.** C develops memory awareness, explicit ownership, data layout intuition, procedural architecture, and debugging discipline. You switch to C++ at Stage 5, once the projects are complex enough that you can see what C++ is buying and what it costs. Almost every C++ feature automates something you'll have done by hand: destructors automate the cleanup you tracked manually, templates automate your macro-generic containers, virtual functions automate your function-pointer vtables. Learn them after the pain and they're tools. Learn them before and they're magic you cargo-cult.

**4. Reimplement deliberately.** When learning a new layer, port a game you already understand. Asteroids in SDL2D → Asteroids in OpenGL. The design is known, so the new variable is isolated.

**5. Measure before optimizing.** Warnings, sanitizers, assertions, debugger, frame timings, CPU profiler, GPU timers. Never optimize on intuition.

**6. Keep a technical journal.** Per project: what was hard, which bug cost the most time, what you misunderstood, what you'd redesign, what's worth reusing. This is where learning consolidates and where your portfolio write-ups come from.

**7. Sanitizers from day one.** `-Wall -Wextra -g -fsanitize=address,undefined`. C and C++ will happily let you ship memory corruption for months.

---

# 3. Toolchain

Pick once, stop thinking about it.

| Area | Choice |
|---|---|
| OS | Linux primary, but produce Windows builds from Stage 2 onward (mingw-w64 cross-compile) |
| Compiler | Clang (GCC fine) |
| C standard | C17 |
| C++ standard | C++20 (C++17 if a target platform lags) |
| Build | Handwritten Makefile through Stage 4, CMake from Stage 5 |
| Debugger | gdb/lldb, plus `rr` for reverse debugging |
| Dynamic analysis | ASan, UBSan, valgrind |
| Static analysis / format | clang-tidy, clang-format |
| Profiling | Tracy (has a C API), perf |
| Platform + graphics | SDL3, then SDL3 + OpenGL 3.3 core |
| Images | stb_image |
| Fonts | stb_truetype |
| Audio | miniaudio |
| JSON | jsmn or cJSON |
| glTF | cgltf |
| Debug UI | microui or Nuklear in C, Dear ImGui once in C++ |
| Levels | Tiled |
| 3D assets | Blender → glTF |
| VCS | git, commit per feature |

**On SDL3 rather than raylib:** you keep one platform layer from month two to month twenty-six instead of switching mid-journey. The cost is that SDL gives you no text or audio conveniences, so pull in stb_truetype and miniaudio at Pong. That's a good trade; those are libraries you'd use forever anyway.

---

# 4. Roadmap at a glance

| Stage | Duration | Deliverable |
|---|---:|---|
| 0. C foundations | 3–5 weeks | Terminal games + five hand-built data structures |
| 1. SDL3 2D fundamentals | 5–7 weeks | Pong, Breakout, Asteroids |
| 2. First serious 2D game | 5–6 months | **A finished, released 2D game** |
| 3. Performance + architecture | 2 months | High-entity simulation with profiler evidence |
| 4. Software rasterizer | 2 months | 3D rendering with no GPU |
| 4B. OpenGL renderer | 6–8 weeks | Your own 2D renderer on the GPU |
| 5. C++ transition | 2–3 months | Math library, rasterizer, and game systems ported |
| 6. 3D foundations | 2 months | Explorable 3D scene |
| 6B. First 3D game | 3–4 months | **A finished 3D game** |
| 7. Shipping | 3–4 months | **A polished, published vertical slice** |
| 8. Specialization | ongoing | Chosen path + merged open-source contributions |

Three finished games. That's the point of the whole document.

---

# Stage 0 — C foundations
**3–5 weeks. No external libraries.**

### 0A: Terminal Game of Life
2D grids in 1D arrays (`idx = y*w + x` and why that beats `grid[y][x]`), double buffering and why you can't update in place, structs, enums, `static` for file scope, header/source split, a Makefile with debug and release targets, timing with `clock_gettime`.

### 0B: Terminal Snake
Raw terminal input (`termios`, ANSI escapes), fixed update rate, game state, collision, score, and restoring terminal mode on exit via `atexit`.

**Pitfalls:** flicker from `clear` instead of cursor-home redraw; forgetting terminal cleanup and leaving the shell unusable.

### Five small C exercises
Build each once, badly, to understand its failure modes rather than to make a library:

1. Dynamic array
2. String slice (`{ char *data; size_t len; }`) instead of null-terminated strings
3. **Arena allocator** (the most important one; read Ryan Fleury's "Untangling Lifetimes")
4. Hash table
5. Ring buffer

### Gate
Explain without looking anything up: stack vs heap, pointer vs value, array decay, struct layout and padding, the lifetime of an allocation, why UB is dangerous, and the full path from source file to running executable.

---

# Stage 1 — SDL3 2D fundamentals
**5–7 weeks. Three finished toys.**

Do not study SDL for weeks before making anything. Read enough to open a window and blit a rectangle, then start.

### Project 1: Pong (~2 weeks)
SDL init, window, renderer, event loop, input polling vs events, `dt` and why `pos += speed` is a bug, AABB overlap, paddle reflection by hit offset, a `MENU / PLAYING / PAUSED / GAME_OVER` state machine as a tagged enum, audio via miniaudio, text via stb_truetype.

**Finish it**, including the menu and the restart. This is where the habit forms.

### Project 2: Breakout (~2 weeks)
Fixed-capacity entity arrays with an `active` flag, collision *resolution* (which axis, what normal, how to depenetrate), levels loaded from files, a sprite atlas with source rects, particles as a fixed ring of structs, powerups, HUD.

**Pitfalls:** the ball tunneling through bricks at speed. Note it, don't fix it yet; Stage 2 solves it properly. Resolving X and Y simultaneously and sticking in corners.

### Project 3: Asteroids (~2–3 weeks)
Write your own `Vec2` module with add, sub, scale, dot, length, normalize, rotate, lerp, and unit-test it. Then rotation and thrust, screen wrapping including the edge-drawn copies, object pools with a free list instead of malloc-per-bullet, SAT or circle collision, wave spawning.

Then spend a week on **game feel**: screen shake, hitstop, muzzle flash, particle burst, input buffering. Watch Jan Willem Nijman's *Game Feel* talk and apply five specific things from it.

### Gate
From an empty file, without a tutorial, write init → input → update → render → shutdown and get a controllable sprite on screen.

---

# Stage 2 — Your first serious 2D game
**5–6 months. The most important stage in the document.**

Not another toy. A small, complete, released game. Recommended: a 2D action platformer. Alternatives: top-down action game, dungeon crawler, twin-stick shooter, small metroidvania.

Keep the scope deliberately small and the polish high.

### Character
Acceleration/deceleration split, air control, variable jump height by hold duration, **coyote time**, **jump buffering**, apex hang, corner correction, animation states (idle/run/jump/fall/land) driven by an explicit state machine, hit reactions and invulnerability frames.

Every one of these numbers goes in a config file you can tune without recompiling. Feel is found by tuning, not by design.

### World
Tilemaps with multiple layers, culled drawing of visible tiles only, a camera with follow smoothing, a dead zone, look-ahead, and clamping to level bounds, parallax layers, level transitions, checkpoints.

### Collision
**Swept AABB against the tile grid**, resolved axis by axis. This properly solves the tunneling you hit in Breakout. Add one-way platforms and moving platforms, including the carry-the-player problem. Slopes optional.

**Pitfalls:** `floorf` rather than truncation for tile lookups, especially at negative coordinates; camera jitter from rendering at fractional pixel offsets; X-before-Y vs Y-before-X changing corner behavior.

### Enemies and navigation
Two or three enemy types: a patroller, a chaser, a ranged attacker. Explicit finite state machines, not a framework.

**Restored: implement A\* on the tile grid with a binary heap you write yourself.** Two days of work. It shows up constantly in real projects and in interviews, and it forces you to build a priority queue in anger. Use it for the chaser, or for a companion, or for enemy repositioning. Also write a seeded PRNG (xorshift or PCG); `rand()` is not reproducible across platforms and reproducibility is worth more than you'd think.

### Content pipeline
Author levels in Tiled, export JSON, write the loader yourself. Enemy stats, damage values, and tuning constants live in external files.

**Restored: hot code reloading.** Build the gameplay layer as a shared library, keep all state in an arena owned by the platform layer, reload on file change. Roughly two days of work in C, and it pays back within weeks on a project this long. This is the Handmade Hero technique and it changes how fast you can iterate on feel. If the platform fiddling stalls you for more than three days, settle for hot-reloading data and shaders only and move on.

**Restored: serialization done once, properly.** A save file with a version header, a binary format plus a text format for debugging, and **reference fixup on load**. If your entities point at each other, you have to remap those references after deserialization. Doing this once teaches you more about ownership and identity than a month of reading.

### Minimum finished scope
6–10 levels, 2–3 enemy types, one boss or major encounter, checkpoints, sound effects, music, particles, menus, settings, save, and a real win condition.

### The rule
**Ship it.** It does not need to be good. It needs to be complete, downloadable, and playable by a stranger. Put it on itch.io.

### Gate
Someone you don't know plays it start to finish and doesn't ask you a question.

---

# Stage 3 — Performance and architecture
**~2 months.**

### Project 4: Horde / bullet-hell simulation
Thousands of simultaneous entities. Naive approaches break visibly, which is the entire pedagogical point.

**Topics**
- **Fixed timestep with an accumulator plus render interpolation.** Read Gaffer On Games, "Fix Your Timestep!"
- **Data-oriented design.** Convert entities from array-of-structs to struct-of-arrays and measure. Understand cache lines and why per-system loops beat per-entity loops. Do not assume SoA always wins; find the case where it doesn't.
- **Spatial partitioning:** a uniform grid or spatial hash for broadphase. Log collision-test counts before and after.
- **Arena allocators in anger:** one allocation at startup, bump-pointer suballocation, a per-frame scratch arena reset at zero cost.
- **Restored: generational handles.** `struct Handle { u32 index; u32 generation; }` instead of raw pointers into your entity arrays, so a stale reference is *detectable* rather than a use-after-free. This is the single most important architectural idea in gameplay code, it's about 50 lines, and it's a required deliverable of this project. Retrofit it into your Stage 2 game afterward if you have the appetite.
- **Profiling:** Tracy or perf, plus an in-game frame-time graph and per-system timing breakdown.
- Bullet patterns described as **data** (spiral, ring, aimed, burst) rather than a function per pattern.

### Gate
Ten thousand active entities at a stable 60 fps in a release build, an on-screen frame-time overlay, and a README that answers "why was this loop slow" with profiler evidence rather than a guess.

---

# Stage 4 — Software rasterizer
**~2 months. No GPU at all.**

The highest-leverage project in the document. Render a textured, lit, depth-buffered model into a pixel buffer you own, then blit it with SDL. After this, nothing in the GPU pipeline is mysterious.

**Topics:** model → world → view → clip → perspective divide → NDC → screen, each transform derived rather than copied. Homogeneous coordinates and why W matters. Bresenham lines, then triangle filling with **barycentric coordinates**. The **z-buffer** and depth precision. **Perspective-correct interpolation** (interpolate 1/w, and look at the artifact when you don't). Backface culling. Near-plane clipping, the case you cannot skip. Flat → Gouraud → Phong shading. OBJ parsing, normals, UVs, bilinear texture sampling.

**Reference:** Sokolov's `tinyrenderer` lessons. Do the lessons, don't clone the repo.

**Done when:** a spinning textured model with per-pixel lighting, correct occlusion, and a movable camera, in software, at an interactive framerate. Do not spend a month making it beautiful.

---

# Stage 4B — OpenGL renderer
**6–8 weeks. SDL3 + OpenGL 3.3 core.**

Rebuild your 2D renderer on the GPU, and port Asteroids onto it so the game logic is a known quantity.

**Topics:** context creation, the OpenGL state machine mental model, VAO/VBO/EBO, GLSL vertex and fragment shaders with proper error reporting and hot reload from disk, uniforms and attributes, textures (formats, filtering, mipmaps, wrap modes, premultiplied alpha), blending, depth testing, framebuffers and render targets, **sprite batching** (accumulate quads, flush on texture change), and orthographic projection with your own `Mat4`.

Keep the architecture as: game → renderer API → OpenGL backend. Nothing more.

**Add exactly one advanced feature**, not four: bloom, dynamic 2D lighting with normal maps, 2D shadow casting, or a CRT post-process. One is enough unless you already know you want a graphics career, in which case do two and write them up.

**Pitfalls:** stale state producing invisible geometry; Y-axis direction confusion; linear vs sRGB color space, which everyone ships wrong once; using `glGetError` instead of `glDebugMessageCallback`.

**Done when:** the ported game is indistinguishable from the SDL2D version at equal or better framerate, with a single-digit draw-call count per frame.

---

# Stage 5 — C++ transition
**2–3 months.**

Do not learn C++ by reading the language. Port code you already understand, so every difficulty you hit is a language difficulty rather than a design one.

### 5A: Port the math library
`Vec2/3/4`, `Mat4`, quaternions. Learn references, const correctness, constructors and destructors, value semantics, **operator overloading** (the single best fit for C++ in this domain), `constexpr`, and templates over scalar type.

The goal is `position += velocity * dt;` instead of `position = vec_add(position, vec_scale(velocity, dt));`

### 5B: Port the rasterizer
Tight loops, so this is where you learn what C++ costs. **RAII and destructors** (wrap a file handle, a GL texture, an arena, and watch the error handling disappear). Move semantics, rvalue references, `std::move`, copy elision, the rule of zero/three/five. The STL: `vector`, `array`, `span`, `string_view`, `optional`, algorithms, and where each is slow. Smart pointers, with `unique_ptr` as default and `shared_ptr` as a rare exception. **Custom allocators**: plug your arena into a container via a custom allocator or `std::pmr::monotonic_buffer_resource`. Proper CMake targets with `PUBLIC`/`PRIVATE` propagation. Tests with doctest or Catch2.

Do not assume C++ is automatically slower or faster. Measure in release builds only; debug-build STL is roughly 10x slower and will give you wrong conclusions.

### 5C: Port selected systems from your 2D game
Use C++ where it improves clarity. Keep C-like data-oriented designs where they're still better. Your generational handles stay; they are still the right answer for entities, and no smart pointer replaces them.

### The dialect that actually gets used
This matters as much as the features. Studios do not write enterprise C++.

- Exceptions and RTTI are frequently disabled (`-fno-exceptions -fno-rtti`). Errors are return codes, `std::optional`, or asserts. Search "Orthodox C++" for the philosophy.
- The STL is often restricted or replaced (EASTL, in-house containers) because allocation behavior matters.
- `shared_ptr`, deep inheritance, and heavy metaprogramming are treated with suspicion in hot code.
- **Watch Mike Acton's "Data-Oriented Design and C++" (CppCon 2014).** It's the bridge from what you learned in C to how good C++ game code is written. Almost nothing in it will surprise you, which is exactly the point.
- Read *A Tour of C++* cover to cover in a weekend, then *Effective Modern C++* over the following month.

### Gate
Explain, without slogans: object lifetime, move semantics, `std::vector` reallocation, virtual dispatch and vtable layout, template instantiation cost, allocator behavior, and the exception vs error-code tradeoff. Everything builds clean under `-Wall -Wextra -fno-exceptions -fno-rtti`. Name three places C++ improved the code and two where it made it worse.

---

# Stage 6 — 3D foundations
**~2 months.**

### Project 7: 3D scene explorer
A small first-person walkable scene.

**Topics:** free-fly and FPS cameras with pitch clamping, the view matrix derived from position and basis vectors rather than copied, quaternions (and why gimbal lock happens, and when Euler angles are still fine), perspective projection, depth testing and z-fighting, face culling, mesh loading via **glTF with cgltf**, materials, directional and point and spot lights with Blinn-Phong, normal mapping and tangent space, a cubemap skybox, and **frustum culling** with planes extracted from the view-projection matrix.

Scene representation: a flat array of nodes with parent indices resolved in one pass, not a pointer-chasing scene graph.

**Done when:** you walk around a multi-mesh, multi-light scene at 60 fps with a drawn-vs-culled counter on screen.

---

# Stage 6B — Your first 3D game
**3–4 months. Your second serious game.**

A 3D platformer or a small arena shooter. Keep it small.

**Collision:** sphere and capsule tests, raycasts, swept movement, sliding along surfaces, ground detection, step-up, slope limits, and a BVH or loose octree for static geometry broadphase. Read Ericson, *Real-Time Collision Detection*, chapters 4–5. Do not build a general-purpose physics engine.

**Character and camera:** a controller state machine (grounded/air/jumping/sliding) and a spring-arm camera that sweeps so it doesn't clip through walls.

**Animation:** glTF skins, joint hierarchies, inverse bind matrices, GPU skinning in the vertex shader, blending and crossfade.

**Rendering:** basic shadow mapping, then the bias/acne/peter-panning fixes and PCF.

**Architecture decisions you now have to make for real:** component composition vs inheritance; a minimal ECS (write one, or read EnTT's design) vs your handle arrays; virtual functions vs tagged unions vs type erasure; where templates pay for themselves. Justify each in the README.

**Multithreading:** add a small job system with `std::thread` and atomics only if the game gives you a reason, but do learn the memory model well enough not to write a data race. Threading questions come up in interviews often enough to be worth the exposure.

**Pipeline:** author levels in Blender, export glTF, define spawn points and triggers via custom properties.

**Gate:** a friend downloads, runs, understands, plays, and finishes it without your help.

---

# Stage 7 — Shipping
**3–4 months. 15–30 minutes of polished gameplay.**

Build the best small game you can from the systems you've accumulated. A vertical slice, not a dream project.

**Production systems only:** settings, controller support, key rebinding, resolution and fullscreen, save/load, audio mixing options, accessibility basics, robust asset loading with an offline packer, error logging, crash-safe behavior, packaging, and release builds for Linux and Windows (plus web via Emscripten if it fits).

**Distribution:** itch.io, screenshots, a 60–90 second trailer, a README, a technical article, and a postmortem.

**Definition of done:** a stranger downloads it and finishes it without asking you a question.

---

# Stage 8 — Specialization
**Ongoing. Choose a direction rather than automatically building a bigger engine.**

### Required regardless of path: open-source contribution
**~80–150 h spread over months.** This is the single most credible evidence that you can work on a team, and it's the one thing no solo project can substitute for. It proves you can navigate a codebase you didn't write, follow conventions you didn't choose, take review from strangers, and land a change.

Targets: Godot, SDL, bgfx, raylib, Box2D or Jolt, Dear ImGui, o3de, or Blender if pipeline work appeals.

Process: build from source, run their tests, read `CONTRIBUTING.md` twice, fix a small documented bug first, then take something with design discussion attached.

Skills it builds that nothing else here does: navigating 200k+ lines with clangd and a debugger rather than by reading, CI and platform matrices, writing a reviewable change description, responding to feedback without defensiveness, backwards compatibility thinking.

**Target: three merged PRs, at least one non-trivial.** Start this during Stage 7, not after.

### Path A — Engine / systems
Allocators, ECS and data-oriented architecture at scale, job systems and task graphs, asset pipelines, serialization and schema versioning, platform layers, profiling infrastructure. Read *Game Engine Architecture* (Gregory), *Foundations of Game Engine Development* (Lengyel), Handmade Hero.

### Path B — Graphics
GPU architecture, modern OpenGL, then **Vulkan** (only now, because you know what OpenGL was hiding): command buffers, synchronization, descriptor sets, pipelines, memory management, compute. Then PBR with IBL, cascaded shadow maps, clustered or forward+ shading, GPU-driven rendering, render graphs. *Real-Time Rendering* 4e becomes your permanent reference. `vkguide.dev` is the practical Vulkan path.

### Path C — Gameplay
One strong **Unreal Engine C++** project, since a large share of studios use it and postings name it. `UObject`/`AActor`/`UActorComponent`, the reflection system (`UPROPERTY`, `UFUNCTION`, UHT) and garbage collection, the Blueprint/C++ boundary, the Gameplay Ability System, animation blueprints, replication and network authority, Unreal Insights, custom editor tooling, cooking and packaging. Your from-scratch background makes this fast; Unreal C++ is its own dialect with its own memory rules, and you'll understand *why* each rule exists.

### Path D — Indie
Game design, iteration speed, art and audio pipelines, level design, production, marketing, shipping. Godot becomes a pragmatic tool at this point. The goal stops being proving you can write an engine.

---

# Continuous tracks

### C
Memory: ownership, lifetime, alignment, padding, arenas, pools, temporary allocations. Data: slices instead of null-terminated strings, ring buffers, hash tables, packed data, **generational handles**. Interfaces: function pointers, tagged unions, explicit state machines, vtable-style structs. Performance: cache locality, branch prediction, allocation costs, SIMD awareness. Tooling: gdb/lldb, sanitizers, perf, rr.

### C++ (from Stage 5)
Value semantics → RAII and lifetime → the STL and its costs → move semantics at the byte level → templates as controlled generic code rather than metaprogramming → custom allocators in standard containers → the memory model and concurrency → build-time discipline (forward declarations, pImpl, unity builds). Keep your C instincts: explicit lifetimes, flat data, no hidden allocation in the frame loop.

### Mathematics
Learn each piece when a game demands it: vectors → dot and cross products and what they're *for* → interpolation and easing → matrices → transformations → coordinate spaces → projections → planes and rays → collision geometry → quaternions → numerical stability and epsilon comparison. *3D Math Primer* (Dunn & Parberry) is the right book. Don't study advanced math with no visual problem attached.

### Debugging
Become unusually good at this; it's a bigger differentiator than knowing more APIs. Assertions everywhere, ASan and UBSan always, watchpoints and conditional breakpoints, core dumps, `rr` for reverse debugging, Tracy for frame profiling, in-game overlays for frame time and memory.

**Build deterministic input replay** during Stage 2 or 3: record `input[frame]`, replay the whole game. You get reproducible bugs, regression tests, easier profiling, and a foundation for multiplayer experiments later. It is one afternoon of work and it will save you weeks.

---

# What NOT to learn yet

These are detours, not gates. Adding them early is the most common way this journey stalls.

Vulkan. A full ECS framework. A custom scripting language. A general physics engine. Networking. Deferred rendering, render graphs, PBR. Advanced animation systems. A sophisticated editor. Unreal. Large-scale engine architecture.

Every one of them is legitimate. None is a prerequisite for anything above. Add them when a project makes them useful, which for most of them means Stage 8.

*(Note the exceptions this document makes deliberately: hot reload, generational handles, A\*, and serialization are cheap, and each pays for itself inside the same project that introduces it. That's the test for whether an "advanced" topic belongs early.)*

---

# Optional detours

- **Networking** (after Stage 2 or 6B): UDP sockets, packet serialization, client-side prediction, server reconciliation, snapshot interpolation, lag compensation. Gaffer On Games' series is canonical. Your deterministic replay system makes this much easier.
- **Physics** (after Stage 2): impulse resolution, restitution, friction, constraints, sequential impulse solvers, GJK/EPA. Erin Catto's GDC slides and the Box2D Lite source. Only if physics itself interests you.
- **Scripting** (any time after Stage 5): *Crafting Interpreters* part II. Excellent and entirely optional.
- **Audio DSP**: mixing from scratch, resampling, reverb, dynamic music layering.

---

# Reading list

Do not read any of these cover to cover before coding.

| Resource | When |
|---|---|
| *Modern C* — Gustedt (free) | Stage 0 |
| "Untangling Lifetimes: The Arena Allocator" — Fleury | Stage 0 |
| *Game Programming Patterns* — Nystrom (free online) | Stages 1–2 |
| *Game Feel* talk — Jan Willem Nijman | Stage 1 |
| Gaffer On Games | Stage 3, networking detour |
| Handmade Hero (early episodes) | Stage 2 hot reload, Stage 3 |
| tinyrenderer — Sokolov | Stage 4 |
| LearnOpenGL | Stages 4B, 6, 8B |
| *A Tour of C++* — Stroustrup | Stage 5, one weekend |
| "Data-Oriented Design and C++" — Acton | Stage 5, watch twice |
| *Effective Modern C++* — Meyers | Stage 5 |
| cppreference.com | Ongoing from Stage 5 |
| *3D Math Primer* — Dunn & Parberry | Stages 4, 6 |
| *Real-Time Collision Detection* — Ericson | Stage 6B |
| *Game Engine Architecture* — Gregory | Stage 6B onward; best single interview-prep book here |
| *C++ Concurrency in Action* — Williams | Job system work |
| *Real-Time Rendering* 4e | Path B |
| *Foundations of Game Engine Development* — Lengyel | Paths A and B |

---

# Gates

Answer honestly. A "no" means repeat the ideas in a small side project rather than advancing.

**After Stage 1** — can you write a small 2D game from an empty file with no tutorial open?

**After Stage 2** — did you finish and release it, and did you tune it based on someone else playing it?

**After Stage 3** — can you explain why a data-oriented loop outperforms an object-oriented one, and prove it with a profiler? Can you explain what your generational handles protect against?

**After Stage 4/4B** — can you trace a vertex from your buffer through transformation, clipping, rasterization, depth test, and fragment shading, and draw a textured rotated quad from an empty `main()` in under 200 lines?

**After Stage 5** — can you explain RAII, move semantics, virtual dispatch, vector reallocation, templates, ownership, and allocation behavior without reciting slogans?

**After Stage 6B** — can another person finish your 3D game unassisted?

**After Stage 7** — have you actually shipped? If not, do not start another engine project.

**After Stage 8** — do you have merged PRs in a codebase you don't own, and could you survive a 45-minute technical screen without cramming the night before?

---

# 26-month schedule

Approximate. Not a deadline.

| Months | Focus |
|---|---|
| 1 | C foundations, terminal projects, five data structures |
| 2–3 | SDL3: Pong, Breakout, Asteroids |
| 4–9 | **Serious 2D game** (including hot reload, A\*, serialization) |
| 10–11 | Performance project (including generational handles) |
| 12–13 | Software rasterizer |
| 14–15 | OpenGL renderer |
| 16–18 | C++ transition |
| 19–20 | 3D scene explorer |
| 21–24 | **First 3D game** |
| 25–26 | **Polish and ship** |
| 24 onward | Open-source contributions, specialization path, portfolio |

Stage 2 is budgeted at six months because 6–10 levels, a boss, save, menus, and music immediately after Asteroids realistically takes that long. If it runs longer because you're learning something important, **finish the game** and let the calendar slip. The one thing not to do is abandon it at 80%.

---

# Weekly rhythm

At 10–15 hours per week:

- **7–10 hours** building the game
- **2–3 hours** studying concepts directly attached to what you're building this week
- **1 hour** reading source code, profiling, or writing journal notes

A good week produces something tangible: a new mechanic, a system, a measured performance improvement, a fixed bug, or a playable build. A week that produces only reading is a week to correct.

---

# Portfolio

Three excellent artifacts, not nine mediocre ones.

1. **The 2D game** — gameplay, architecture, collision, animation, tooling, shipping
2. **The renderer / technical demo** (rasterizer plus OpenGL work) — C, memory, graphics, math, profiling
3. **The 3D game** — C++, 3D graphics, collision, animation, performance, production

Plus, from Stage 8: **merged pull requests** with your name on them.

Each project needs a repository, a README that works, GIFs, a short video, build instructions someone can follow, a technical write-up, and a postmortem. The write-ups matter more than the code, because a hiring engineer will read "how I implemented swept collision and the three bugs that cost me a week" and will not clone your repo.

---

# Interview preparation

Start only once you have real projects to discuss, which is around Stage 7.

**C/C++:** lifetime, memory layout, alignment, pointers vs references, move semantics, virtual dispatch and vtable layout, templates, STL costs.

**Math:** dot and cross products and their uses, matrix multiplication order, projection, quaternion basics, ray-plane and ray-triangle intersection, on a whiteboard.

**Game programming:** game loops, fixed timestep, collision, spatial partitioning, animation, cameras, resource management.

**Graphics:** transformations, coordinate spaces, depth buffer, shaders, CPU/GPU synchronization.

**Performance:** be able to answer "why is this code slow" and describe how you'd measure it. This question is asked constantly and most candidates guess.

**Algorithms:** arrays, hash tables, trees, graphs, sorting, binary search, complexity. Moderate depth; some studios run standard tech screens.

Practice the format: 45 minutes, talking while coding, on a problem you haven't seen.

---

# If time runs short, protect these

1. **The serious 2D game** — most important for learning game development at all
2. **The performance project** — most important for systems thinking
3. **The software rasterizer** — most important for understanding graphics
4. **The C++ transition** — most important for becoming employable in this field
5. **The 3D game** — most important for putting it together
6. **A shipped game** — most important professionally
7. **Merged open-source PRs** — most important for joining a team

Everything else is negotiable.

---

# Final philosophy

The goal is not "I completed a game development curriculum." The goal is:

> I can take a game from an empty repository to a finished release, and I understand what the machine is doing underneath it.

You should end able to move freely between gameplay, data structures, memory, the operating system, the renderer, the GPU, and the language, without an engine hiding the parts that matter.

And throughout: **make games**. Do not wait until you feel ready.
