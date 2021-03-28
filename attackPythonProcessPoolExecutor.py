import concurrent.futures
import time


NB_THREAD = 4


# Returns True if the hashes are the same
def same_hash(dict_hash, shadow):
    return dict_hash in shadow


def main():
    dict_file = open("dict.txt", "r")
    shadow_file = open("shadow.txt", "r")

    my_dict = []
    for line in dict_file:
        tmp = line.split("\t")
        my_dict.append(tmp[1])

    my_hashes_to_crack = []
    for line in shadow_file:
        my_hashes_to_crack.append(line)

    start = time.perf_counter()
    # Number of threads : max_workers
    with concurrent.futures.ProcessPoolExecutor(max_workers=NB_THREAD) as executor:
        results = executor.map(same_hash, my_dict, [my_hashes_to_crack]*len(my_dict))
    finish = time.perf_counter()

    # Count the number of correct guesses
    count = 0
    for result in results:
        if result:
            count += 1
    print("Number of threads : "+str(NB_THREAD))
    print("Number of correct guesses : "+str(count))
    print(f'Time to compare the hashes : {round(finish-start, 2)} second(s)')


if __name__ == '__main__':
    main()
