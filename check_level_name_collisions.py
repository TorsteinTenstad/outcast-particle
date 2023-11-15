import pathlib

if __name__ == '__main__':
    names = []
    for level_folder in pathlib.Path('game_data/levels').iterdir():
        if not level_folder.is_dir():
            continue
        for level_file in level_folder.iterdir():
            level_name = level_file.stem.split('_')[-1]
            names.append((level_folder.stem, level_name))

    collisions = 0
    just_names = [name for (group, name) in names]
    for (group, name) in names:
        if just_names.count(name) > 1:
            print(f'Name {name} in {group} is not unique')
            collisions += 1

    print(
        f'Found {collisions} name collisions across {len(names)} levels in {len(set([group for (group, name) in names]))} groups')
