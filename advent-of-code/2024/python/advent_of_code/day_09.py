
def solve(lines):
    res1 = 0
    res2 = 0

    disk_repr = [int(c) for c in lines[0]]

    is_space = False
    disk = []
    disk_id = 0

    for d in disk_repr:
        if is_space:
            disk.extend([-1 for i in range(d)])
            is_space = False
        else:
            disk.extend([disk_id for i in range(d)])
            disk_id += 1
            is_space = True

    disk_copy = disk.copy()

    back_i = len(disk) - 1

    for i in range(len(disk)):
        if disk[i] == -1:
            if back_i < i:
                break
            while disk[back_i] == -1:
                back_i -= 1
            disk[i] = disk[back_i]
            disk[back_i] = -1
            back_i -= 1

    checksum = 0
    for i in range(len(disk)):
        c = disk[i]
        if c == -1:
            break
        checksum += i * c

    res1 = checksum

    disk = disk_copy
    i = len(disk) - 1
    disk_id = disk[i]

    while disk_id > 0:
        i = len(disk) - 1
        while i >= 0 and disk[i] != disk_id:
            i -= 1

        file_len = 0
        while i >= 0 and disk[i] == disk_id:
            i -= 1
            file_len += 1
        file_idx = i + 1

        j = 0
        while True:
            while j < file_idx and disk[j] != -1:
                j += 1
            if j >= file_idx:
                break

            space_idx = j
            space_len = 0
            while j < file_idx and disk[j] == -1:
                space_len += 1
                j += 1

            if space_len >= file_len:
                for k in range(file_len):
                    disk[space_idx + k] = disk[file_idx + k]
                    disk[file_idx + k] = -1
                break
        disk_id -= 1

    checksum = 0
    for i in range(len(disk)):
        c = disk[i]
        if c == -1:
            continue
        checksum += i * c

    res2 = checksum

    return res1, res2
