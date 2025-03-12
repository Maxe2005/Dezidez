from PIL import Image, ImageEnhance

def change_hue(image_path, output_path, hue_factor):
    # Charger l'image
    image = Image.open(image_path).convert("RGBA")

    # Convertir en HSV
    image_hsv = image.convert("HSV")

    # Modifier la teinte
    hue, saturation, value = image_hsv.split()
    hue = hue.point(lambda i: (i + hue_factor) % 256)

    # Recomposer l'image
    image_hsv = Image.merge("HSV", (hue, saturation, value))
    image = image_hsv.convert("RGBA")

    # Sauvegarder l'image modifiée
    image.save(output_path, format='PNG')


def apply_color_filter(image_path, output_path, color_factor):
    # Charger l'image
    image = Image.open(image_path).convert("RGBA")

    # Appliquer un filtre de couleur
    enhancer = ImageEnhance.Color(image)
    image = enhancer.enhance(color_factor)

    # Sauvegarder l'image modifiée
    image.save(output_path, format='PNG')

def replace_color(image_path, output_path, old_color, new_color, tolerance=30):
    # Charger l'image
    image = Image.open(image_path).convert("RGBA")
    image = image.convert("RGBA")

    # Obtenir les données de l'image
    datas = image.getdata()

    new_data = []
    for item in datas:
        # Remplacer la couleur si elle est proche de old_color
        if all(abs(item[i] - old_color[i]) < tolerance for i in range(3)):
            new_data.append((new_color[0], new_color[1], new_color[2], item[3]))
        else:
            new_data.append(item)

    # Mettre à jour les données de l'image
    image.putdata(new_data)

    # Sauvegarder l'image modifiée
    image.save(output_path, format='PNG')

# Utilisation de la fonction
replace_color('Ressources/maison.png', 'tests/maison1.png', old_color=(0, 0, 0), new_color=(255, 0, 0))

# Utilisation de la fonction
apply_color_filter('Ressources/maison.png', 'tests/maison2.png', color_factor=2.0)

# Utilisation de la fonction
change_hue('Ressources/maison.png', 'tests/maison3.png', hue_factor=50)
