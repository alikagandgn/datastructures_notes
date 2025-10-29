// Ad: ALİ KAĞAN 
// Soyad: DOĞAN
// Okul No: 232523060
// Sınıf: 3. SINIF
// Öğrenim Türü (Örgün/İkinci Öğretim): İKİNCİ ÖĞRETİM


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 1000
#define MINV 1
#define MAXV 10000
#define PRINT_ARRAYS 0

/* -------------------- Yardımcı Fonksiyonlar -------------------- */

void fill_random(int *a, int n, int minv, int maxv) {
    for (int i = 0; i < n; i++) {
        a[i] = minv + rand() % (maxv - minv + 1);
    }
}

void copy_array(const int *src, int *dst, int n) {
    memcpy(dst, src, n * sizeof(int));
}

int is_sorted(const int *a, int n) {
    for (int i = 1; i < n; i++) if (a[i-1] > a[i]) return 0;
    return 1;
}

void print_array(const int *a, int n, const char *title) {
#if PRINT_ARRAYS
    if (title) printf("%s\n", title);
    for (int i = 0; i < n; i++) {
        printf("%d", a[i]);
        if (i != n-1) printf(" ");
    }
    printf("\n\n");
#else
    (void)a; (void)n; (void)title; // uyarı sustur
#endif
}

/* -------------------- Insertion Sort -------------------- */

void insertion_sort(int *a, int n) {
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j+1] = a[j];
            j--;
        }
        a[j+1] = key;
    }
}

/* -------------------- Merge Sort -------------------- */

static void merge(int *a, int l, int m, int r, int *tmp) {
    int i = l, j = m+1, k = l;
    while (i <= m && j <= r) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else              tmp[k++] = a[j++];
    }
    while (i <= m) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (i = l; i <= r; i++) a[i] = tmp[i];
}

static void mergesort_rec(int *a, int l, int r, int *tmp) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergesort_rec(a, l, m, tmp);
    mergesort_rec(a, m+1, r, tmp);
    merge(a, l, m, r, tmp);
}

void merge_sort(int *a, int n) {
    int *tmp = (int*)malloc(n * sizeof(int));
    if (!tmp) { fprintf(stderr, "Bellek hatasi!\n"); exit(1); }
    mergesort_rec(a, 0, n - 1, tmp);
    free(tmp);
}

/* -------------------- Quick Sort -------------------- */

static int partition(int *a, int l, int r) {
    int pivot = a[r];
    int i = l - 1;
    for (int j = l; j < r; j++) {
        if (a[j] <= pivot) {
            i++;
            int t = a[i]; a[i] = a[j]; a[j] = t;
        }
    }
    int t = a[i+1]; a[i+1] = a[r]; a[r] = t;
    return i + 1;
}

static void quicksort_rec(int *a, int l, int r) {
    if (l >= r) return;
    int p = partition(a, l, r);
    quicksort_rec(a, l, p - 1);
    quicksort_rec(a, p + 1, r);
}

void quick_sort(int *a, int n) {
    quicksort_rec(a, 0, n - 1);
}

/* -------------------- Zaman Ölçümü -------------------- */

double time_in_seconds(void (*sort_fn)(int*, int), int *arr, int n) {
    clock_t start = clock();
    sort_fn(arr, n);
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

/* -------------------- main -------------------- */

int main(void) {
    srand((unsigned)time(NULL));

    int original[N];
    fill_random(original, N, MINV, MAXV);
    print_array(original, N, "Rastgele 1000 elemanli dizi:");

    int a1[N], a2[N], a3[N];
    copy_array(original, a1, N);
    copy_array(original, a2, N);
    copy_array(original, a3, N);

    // Insertion Sort
    double t_insertion = time_in_seconds(insertion_sort, a1, N);
    if (!is_sorted(a1, N)) { printf("Insertion Sort hatali!\n"); return 0; }

    // Merge Sort
    double t_merge = time_in_seconds(merge_sort, a2, N);
    if (!is_sorted(a2, N)) { printf("Merge Sort hatali!\n"); return 0; }

    // Quick Sort
    double t_quick = time_in_seconds(quick_sort, a3, N);
    if (!is_sorted(a3, N)) { printf("Quick Sort hatali!\n"); return 0; }

    print_array(a3, N, "Siralanmis dizi (Quick Sort sonrasi):");

    printf("Insertion Sort suresi : %.6f saniye\n", t_insertion);
    printf("Merge Sort suresi     : %.6f saniye\n", t_merge);
    printf("Quick Sort suresi     : %.6f saniye\n", t_quick);

    for (int i = 0; i < N; i++) {
        if (a1[i] != a2[i] || a2[i] != a3[i]) {
            printf("UYARI: Siralama sonuclari farkli!\n");
            break;
        }
    }

    /*
    Tartışma:
    - Insertion Sort: O(n^2). Kucuk n'lerde basit ama 1000 elemanda genelde en yavaş.
    - Merge Sort: O(n log n). Kararlı, büyük veri için hızlı ve istikrarlı; ek bellek kullanır.
    - Quick Sort: Ortalama O(n log n), pratikte cok hızlı; pivot secimine bagli olarak en iyi sonucu verir.

    Genelde beklenen: Quick Sort ≈ Merge Sort << Insertion Sort.
    Derleyici ve donanıma gore sayılar degişebilir.
    */

    return 0;
}
