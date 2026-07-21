# codegen

Generates Rust and C++ source files from an HCL robot configuration using [Tera](https://keats.github.io/tera/) templates.

## Quick start

```bash
# From workspace root
cargo run --bin codegen \
  -i hcl/robot.hcl -d hcl \
  -g rust.tera generated/src/generated.rs \
  -g cpp_header.tera generated/cpp/generated.h \
  -g cpp_source.tera generated/cpp/generated.cpp
```

## Arguments

| Flag | Long | Takes | Description |
|---|---|---|---|
| `-i` | `--input` | `<PATH>` | Path to the HCL robot config. Defaults to `../../hcl/robot.hcl`. |
| `-d` | `--template-dir` | `<DIR>` | Directory containing `*.tera` templates. Defaults to `hcl`. |
| `-g` | `--gen` | `<TEMPLATE> <OUTPUT>` | Template → output pair. Repeatable. Required. |

## How it works

1. Parse the HCL config once — messages, endpoints, robot metadata.
2. Build a single Tera template context with all parsed data.
3. For each `-g TEMPLATE OUTPUT` pair, render the named template to the given output file.

The HCL config is loaded once, so adding more `-g` pairs is cheap — only the template rendering step repeats.

## Available templates

All templates live in the `hcl/` directory (or wherever `-d` points):

| Template | Purpose |
|---|---|
| `rust.tera` | Rust message structs with serde/CBOR support |
| `cpp_header.tera` | C++ header with message classes and endpoint constants |
| `cpp_source.tera` | C++ source with serialization implementations |

## HCL input

The input file describes a robot with named endpoints and messages. See `hcl/robot.hcl` for the full schema. Key concepts:

- **robot** — top-level block: name, model, multicast settings.
- **endpoint** — named component with `services`, `events`, `replies`, and `subscribes`.
- **message** — named struct with typed `field` entries (uint32, int32, float, string, bool, etc.).

## Examples

Single Rust output:
```bash
cargo run --bin codegen -i hcl/robot.hcl -g rust.tera src/messages.rs
```

C++ header and source from one invocation:
```bash
cargo run --bin codegen -i hcl/robot.hcl \
  -g cpp_header.tera include/messages.h \
  -g cpp_source.tera src/messages.cpp
```

All three outputs:
```bash
cargo run --bin codegen -i hcl/robot.hcl \
  -g rust.tera ../generated/src/generated.rs \
  -g cpp_header.tera ../generated/cpp/generated.h \
  -g cpp_source.tera ../generated/cpp/generated.cpp
```
