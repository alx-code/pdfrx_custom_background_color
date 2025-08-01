# pdfrx

[![Build Test](https://github.com/espresso3389/pdfrx/actions/workflows/build-test.yml/badge.svg)](https://github.com/espresso3389/pdfrx/actions/workflows/build-test.yml)

[pdfrx](https://pub.dartlang.org/packages/pdfrx) is a rich and fast PDF viewer plugin for Flutter. It provides ready-to-use widgets for displaying PDF documents in your Flutter applications.

This plugin is built on top of [pdfrx_engine](https://pub.dartlang.org/packages/pdfrx_engine), which handles the low-level PDF rendering using [PDFium](https://pdfium.googlesource.com/pdfium/). The separation allows for a clean architecture where:

- **pdfrx** (this package) - Provides Flutter widgets, UI components, and platform integration
- **pdfrx_engine** - Handles PDF parsing and rendering without Flutter dependencies

The plugin supports Android, iOS, Windows, macOS, Linux, and Web.

## Interactive Demo

A [demo site](https://espresso3389.github.io/pdfrx/) using Flutter Web

![pdfrx](https://github.com/espresso3389/pdfrx/assets/1311400/b076ac0b-e2cb-48f0-8772-9891537ade7b)

## Multi-platform support

- Android
- iOS
- Windows
- macOS
- Linux (even on Raspberry Pi)
- Web (WASM)

## Example Code

The following fragment illustrates the easiest way to show a PDF file in assets:

```dart
import 'package:pdfrx/pdfrx.dart';

...

class _MyAppState extends State<MyApp> {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Pdfrx example'),
        ),
        body: PdfViewer.asset('assets/hello.pdf'),
      ),
    );
  }
}
```

Anyway, please follow the instructions below to install on your environment.

## Getting Started

### Installation

Add this to your package's `pubspec.yaml` file and execute `flutter pub get`:

```yaml
dependencies:
  pdfrx: ^2.1.3
```

**Note:** You only need to add `pdfrx` to your dependencies. The `pdfrx_engine` package is automatically included as a dependency of `pdfrx`.

### Note for Windows

**REQUIRED: You must enable [Developer Mode](https://learn.microsoft.com/en-us/windows/apps/get-started/enable-your-device-for-development#activate-developer-mode) to build pdfrx on Windows.**

The build process uses *symbolic links* which requires Developer Mode to be enabled. If Developer Mode is not enabled:

- The build will fail with an error message
- You will see a link to Microsoft's official instructions
- You must enable Developer Mode and restart your computer before building

Please follow Microsoft's official guide to enable Developer Mode as the exact steps may vary depending on your Windows version.

## Note for Building Release Builds

*Please note that the section is not applicable to Web.*

Because the plugin contains WASM binaries as its assets and they increase the size of the app regardless of the platform.
This is normally OK for development or debugging but you may want to remove them when building release builds.

To do this, do `dart run pdfrx:remove_wasm_modules` between `flutter pub get` and `flutter build ...` on your app project's root directory:

```bash
flutter pub get
dart run pdfrx:remove_wasm_modules
flutter build ...
```

To restore the WASM binaries, run the following command:

```bash
dart run pdfrx:remove_wasm_modules --revert
```

## Customizations/Features

You can customize the behaviors and the viewer look and feel by configuring [PdfViewerParams](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfViewerParams-class.html).

## Deal with Password Protected PDF Files

```dart
PdfViewer.asset(
  'assets/test.pdf',
  // The easiest way to supply a password
  passwordProvider: () => createSimplePasswordProvider('password'),

  ...
),
```

See [Deal with Password Protected PDF Files using PasswordProvider](https://github.com/espresso3389/pdfrx/wiki/Deal-with-Password-Protected-PDF-Files-using-PasswordProvider) for more information.

### Text Selection

The text selection feature is enabled by default, allowing users to select text in the PDF viewer. You can customize the text selection behavior using [PdfTextSelectionParams](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfTextSelectionParams-class.html).

The following example shows how to disable text selection in the PDF viewer:

```dart
PdfViewer.asset(
  'assets/test.pdf',
  params: PdfViewerParams(
    textSelectionParams: PdfTextSelectionParams(
      enabled: false,
      ...
    ),
  ),
  ...
),
```

The text selection feature supports various customizations, such as:

- Context Menu Customization using [PdfViewerParams.buildContextMenu](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfViewerParams/buildContextMenu.html)
- Text Selection Magnifier Customization using [PdfTextSelectionParams.magnifier](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfTextSelectionParams/magnifier.html)

For more text selection customization, see [Text Selection](https://github.com/espresso3389/pdfrx/wiki/%5B2.1.X%5D-Text-Selection).

### PDF Feature Support

- [PDF Link Handling](https://github.com/espresso3389/pdfrx/wiki/PDF-Link-Handling)
- [Document Outline (a.k.a Bookmarks)](https://github.com/espresso3389/pdfrx/wiki/Document-Outline-(a.k.a-Bookmarks))
- [Text Search](https://github.com/espresso3389/pdfrx/wiki/%5B2.1.X%5D-Text-Search)

### Viewer Customization

- [Page Layout (Horizontal Scroll View/Facing Pages)](https://github.com/espresso3389/pdfrx/wiki/Page-Layout-Customization)
- [Showing Scroll Thumbs](https://github.com/espresso3389/pdfrx/wiki/Showing-Scroll-Thumbs)
- [Dark/Night Mode Support](https://github.com/espresso3389/pdfrx/wiki/Dark-Night-Mode-Support)
- [Document Loading Indicator](https://github.com/espresso3389/pdfrx/wiki/Document-Loading-Indicator)
- [Viewer Customization using Widget Overlay](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfViewerParams/viewerOverlayBuilder.html)

### Additional Customizations

- [Double‐tap to Zoom](https://github.com/espresso3389/pdfrx/wiki/Double%E2%80%90tap-to-Zoom)
- [Adding Page Number on Page Bottom](https://github.com/espresso3389/pdfrx/wiki/Adding-Page-Number-on-Page-Bottom)
- [Per-page Customization using Widget Overlay](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfViewerParams/pageOverlaysBuilder.html)
- [Per-page Customization using Canvas](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfViewerParams/pagePaintCallbacks.html)

## Additional Widgets

### PdfDocumentViewBuilder/PdfPageView

[PdfPageView](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfPageView-class.html) is just another PDF widget that shows only one page. It accepts [PdfDocument](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfDocument-class.html) and page number to show a page within the document.

[PdfDocumentViewBuilder](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfDocumentViewBuilder-class.html) is used to safely manage [PdfDocument](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfDocument-class.html) inside widget tree and it accepts `builder` parameter that creates child widgets.

The following fragment is a typical use of these widgets:

```dart
PdfDocumentViewBuilder.asset(
  'asset/test.pdf',
  builder: (context, document) => ListView.builder(
    itemCount: document?.pages.length ?? 0,
    itemBuilder: (context, index) {
      return Container(
        margin: const EdgeInsets.all(8),
        height: 240,
        child: Column(
          children: [
            SizedBox(
              height: 220,
              child: PdfPageView(
                document: document,
                pageNumber: index + 1,
                alignment: Alignment.center,
              ),
            ),
            Text(
              '${index + 1}',
            ),
          ],
        ),
      );
    },
  ),
),
```

## PdfDocument Management

[PdfDocumentViewBuilder](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfDocumentViewBuilder-class.html) can accept [PdfDocumentRef](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfDocumentRef-class.html) from [PdfViewer](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfViewer-class.html) to safely share the same [PdfDocument](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfDocument-class.html) instance. For more information, see [`example/viewer/lib/thumbnails_view.dart`](example/viewer/lib/thumbnails_view.dart).

## API Documentation

### Flutter Widgets (pdfrx)

- [PdfViewer](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfViewer-class.html) - Main PDF viewer widget
- [PdfDocumentViewBuilder](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfDocumentViewBuilder-class.html) - Builder for safe async document loading
- [PdfPageView](https://pub.dev/documentation/pdfrx/latest/pdfrx/PdfPageView-class.html) - Single page display widget

### Low-Level PDF API (pdfrx_engine)

For advanced use cases requiring direct PDF manipulation without Flutter widgets, see the [pdfrx_engine API reference](https://pub.dev/documentation/pdfrx_engine/latest/). This includes:

- [PdfDocument](https://pub.dev/documentation/pdfrx_engine/latest/pdfrx_engine/PdfDocument-class.html) - Core document interface
- [PdfPage](https://pub.dev/documentation/pdfrx_engine/latest/pdfrx_engine/PdfPage-class.html) - Page rendering and manipulation
