# PDF Background Color Customization

This functionality allows you to dynamically change the background color of the PDF viewer, including transparency support.

## Features

- ✅ Dynamic background color change in real-time
- ✅ Transparency support
- ✅ Simple and easy-to-use API
- ✅ Integration with existing controller
- ✅ Compatible with all platforms

## Basic Usage

### 1. Create a Background Color Manager

```dart
final backgroundColorManager = PdfViewerBackgroundColorManager(
  defaultColor: Colors.grey, // Default color
);
```

### 2. Configure the PdfViewer

```dart
PdfViewer(
  documentRef,
  controller: controller,
  params: PdfViewerParams(
    backgroundColorManager: backgroundColorManager,
    // ... other parameters
  ),
)
```

### 3. Change the Background Color

```dart
// Change to a specific color
backgroundColorManager.setColor(Colors.blue);

// Make the background transparent
backgroundColorManager.setTransparent();

// Reset to default color
backgroundColorManager.resetToDefault();

// Change to a custom color
backgroundColorManager.setBackgroundColor(Colors.red.withOpacity(0.5));
```

## Usage with Controller

You can also change the background color using the controller:

```dart
// Change to a specific color
controller.setBackgroundColorTo(Colors.green);

// Make transparent
controller.setTransparentBackground();

// Reset to default color
controller.resetBackgroundColor();

// Change to a custom color
controller.setBackgroundColor(Colors.purple);
```

## Complete Example

```dart
class MyPdfViewer extends StatefulWidget {
  @override
  _MyPdfViewerState createState() => _MyPdfViewerState();
}

class _MyPdfViewerState extends State<MyPdfViewer> {
  final controller = PdfViewerController();
  final backgroundColorManager = PdfViewerBackgroundColorManager(
    defaultColor: Colors.grey,
  );

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('PDF Viewer'),
        actions: [
          // Button to change to blue
          IconButton(
            icon: Icon(Icons.color_lens, color: Colors.blue),
            onPressed: () => backgroundColorManager.setColor(Colors.blue),
          ),
          // Button to make transparent
          IconButton(
            icon: Icon(Icons.visibility_off),
            onPressed: () => backgroundColorManager.setTransparent(),
          ),
          // Button to reset
          IconButton(
            icon: Icon(Icons.refresh),
            onPressed: () => backgroundColorManager.resetToDefault(),
          ),
        ],
      ),
      body: PdfViewer(
        PdfDocumentRefAsset('assets/document.pdf'),
        controller: controller,
        params: PdfViewerParams(
          backgroundColorManager: backgroundColorManager,
        ),
      ),
    );
  }
}
```

## API Reference

### PdfViewerBackgroundColorManager

#### Constructor
```dart
PdfViewerBackgroundColorManager({
  Color defaultColor = Colors.grey,
})
```

#### Methods

- `setColor(Color color)` - Sets a specific color
- `setTransparent()` - Makes the background transparent
- `resetToDefault()` - Resets to default color
- `setBackgroundColor(Color? color)` - Sets a color or null to reset

#### Properties

- `currentColor` - Gets the current color

### PdfViewerController

#### Background Color Methods

- `setBackgroundColorTo(Color color)` - Sets a specific color
- `setTransparentBackground()` - Makes the background transparent
- `resetBackgroundColor()` - Resets to default color
- `setBackgroundColor(Color? color)` - Sets a color or null to reset

## Important Notes

1. **Manager Required**: To use controller methods, you must configure `backgroundColorManager` in `PdfViewerParams`.

2. **Transparency**: The transparent color (`Colors.transparent`) allows you to see the application background behind the PDF.

3. **Performance**: Color changes are immediate and do not affect viewer performance.

4. **Compatibility**: This functionality is compatible with all platforms supported by pdfrx.

## Use Cases

### Dark/Light Theme
```dart
// Change based on theme
if (isDarkMode) {
  backgroundColorManager.setColor(Colors.black);
} else {
  backgroundColorManager.setColor(Colors.white);
}
```

### Custom Colors
```dart
// Color with transparency
backgroundColorManager.setBackgroundColor(Colors.blue.withOpacity(0.3));

// Custom color
backgroundColorManager.setColor(Color(0xFF123456));
```

### Theme Integration
```dart
// Use theme color
backgroundColorManager.setColor(Theme.of(context).scaffoldBackgroundColor);
```

## Technical Implementation

The functionality is implemented using:

1. **ChangeNotifier**: To notify color changes
2. **ListenableBuilder**: To rebuild the widget when color changes
3. **Container color**: To apply background color
4. **Controller methods**: For programmatic access

The color is applied to the main `Container` of the `PdfViewer`, affecting the area around the PDF (the gray areas in the example image). 