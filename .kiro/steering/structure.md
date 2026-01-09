# Project Structure & Organization

## Directory Layout

```
QtNativeUI/
├── include/QtNativeUI/          # Public API headers
├── src/                         # Implementation source code
│   ├── config/                  # Configuration and routing
│   ├── controls/                # UI control implementations
│   ├── private/                 # Private implementation details
│   └── theme/                   # Theming and color system
├── examples/                    # Example applications
├── tests/                       # Test suite
├── resources/                   # Icons and assets
├── translations/                # Internationalization files
├── tools/                       # Development utilities
└── cmake/                       # CMake configuration files
```

## Code Organization Patterns

### Naming Conventions
- **Classes**: PascalCase with 'N' prefix (e.g., `NPushButton`, `NTheme`)
- **Files**: Match class names (e.g., `NPushButton.h`, `NPushButton.cpp`)
- **Private Implementation**: `_p` suffix (e.g., `npushbutton_p.h`)
- **Enums**: Defined in `NEnums.h` and `NIconEnums.h`

### Header Structure
- **Public Headers**: Located in `include/QtNativeUI/`
- **Private Headers**: Located in `src/private/` with `_p.h` suffix
- **Implementation**: Corresponding `.cpp` files in appropriate `src/` subdirectories

### Component Categories

#### Core Controls (`src/controls/`)
- Basic UI elements: buttons, inputs, labels, checkboxes
- Layout components: navigation, tabs, stacked widgets
- Advanced controls: calendars, progress indicators, flyouts

#### Private Implementation (`src/private/`)
- Internal implementation details not exposed in public API
- Custom styles and delegates
- Platform-specific implementations
- Helper classes and utilities

#### Configuration (`src/config/`)
- Application configuration management
- Navigation routing system

#### Theming (`src/theme/`)
- Color system implementation
- Theme management and switching
- Fluent Design color definitions

## File Relationships

- Each public control has a corresponding private implementation
- Styles are implemented as separate classes in `src/private/`
- Platform-specific code uses conditional compilation
- Resource files (icons, translations) are embedded via Qt's resource system

## Development Guidelines

- Keep public API minimal and stable
- Use private implementation pattern (PIMPL) for complex controls
- Separate platform-specific code into dedicated files
- Follow Qt's signal/slot patterns for component communication
- Maintain clear separation between theming and control logic