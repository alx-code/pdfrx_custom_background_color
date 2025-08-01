#!/bin/bash

# Script para publicar pdfrx_custom_background_color en pub.dev
# Uso: ./scripts/publish.sh [version]

set -e

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}🚀 Iniciando publicación de pdfrx_custom_background_color...${NC}"

# Verificar que estamos en el directorio correcto
if [ ! -f "pubspec.yaml" ]; then
    echo -e "${RED}❌ Error: No se encontró pubspec.yaml. Ejecuta este script desde el directorio packages/pdfrx${NC}"
    exit 1
fi

# Verificar que el nombre del paquete es correcto
PACKAGE_NAME=$(grep "^name:" pubspec.yaml | cut -d: -f2 | tr -d ' ')
if [ "$PACKAGE_NAME" != "pdfrx_custom_background_color" ]; then
    echo -e "${RED}❌ Error: El nombre del paquete debe ser 'pdfrx_custom_background_color'${NC}"
    exit 1
fi

# Obtener la versión actual
CURRENT_VERSION=$(grep "^version:" pubspec.yaml | cut -d: -f2 | tr -d ' ')
echo -e "${YELLOW}📦 Versión actual: $CURRENT_VERSION${NC}"

# Si se proporciona una nueva versión, actualizarla
if [ ! -z "$1" ]; then
    NEW_VERSION=$1
    echo -e "${YELLOW}🔄 Actualizando versión a: $NEW_VERSION${NC}"
    
    # Actualizar versión en pubspec.yaml
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        sed -i '' "s/^version: .*/version: $NEW_VERSION/" pubspec.yaml
    else
        # Linux
        sed -i "s/^version: .*/version: $NEW_VERSION/" pubspec.yaml
    fi
    
    echo -e "${GREEN}✅ Versión actualizada${NC}"
fi

# Verificar que dart está instalado
if ! command -v dart &> /dev/null; then
    echo -e "${RED}❌ Error: Dart no está instalado${NC}"
    exit 1
fi

# Verificar que flutter está instalado
if ! command -v flutter &> /dev/null; then
    echo -e "${RED}❌ Error: Flutter no está instalado${NC}"
    exit 1
fi

echo -e "${YELLOW}🔍 Verificando dependencias...${NC}"
flutter pub get

echo -e "${YELLOW}🧪 Ejecutando tests...${NC}"
flutter test

echo -e "${YELLOW}📋 Verificando análisis de código...${NC}"
flutter analyze

echo -e "${YELLOW}📦 Verificando que el paquete se puede publicar...${NC}"
dart pub publish --dry-run

echo -e "${GREEN}✅ Todas las verificaciones pasaron${NC}"
echo -e "${YELLOW}🤔 ¿Quieres publicar ahora? (y/N)${NC}"
read -r response

if [[ "$response" =~ ^([yY][eE][sS]|[yY])$ ]]; then
    echo -e "${GREEN}🚀 Publicando pdfrx_custom_background_color...${NC}"
    dart pub publish
    
    echo -e "${GREEN}✅ ¡Publicación exitosa!${NC}"
    echo -e "${YELLOW}📖 Puedes ver tu paquete en: https://pub.dev/packages/pdfrx_custom_background_color${NC}"
else
    echo -e "${YELLOW}❌ Publicación cancelada${NC}"
fi 