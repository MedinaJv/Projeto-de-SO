#include "sais.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>
using namespace std;

namespace SAIS {

    // Calcula os buckets. Se end for true, armazena o limite final (tail) de cada bucket; caso contrário, o início (head).
    void getBuckets(const vector<int>& s, int K, vector<int>& bucket, bool end) {
        int n = s.size();
        bucket.assign(K, 0);
        for (int i = 0; i < n; i++)
            bucket[s[i]]++;
        int sum = 0;
        for (int i = 0; i < K; i++) {
            sum += bucket[i];
            bucket[i] = end ? sum : (sum - bucket[i]);
        }
    }

    // Realiza a ordenação induzida para posicionar corretamente os sufixos L‑type e S‑type.
    void induceSort(const vector<int>& s, vector<int>& sa, const vector<bool>& t, int K) {
        int n = s.size();
        vector<int> bucket(K);
        // Induz L‑type (varredura da esquerda para a direita)
        getBuckets(s, K, bucket, false);
        for (int i = 0; i < n; i++) {
            int j = sa[i] - 1;
            if (j >= 0 && !t[j])
                sa[bucket[s[j]]++] = j;
        }
        // Induz S‑type (varredura da direita para a esquerda)
        getBuckets(s, K, bucket, true);
        for (int i = n - 1; i >= 0; i--) {
            int j = sa[i] - 1;
            if (j >= 0 && t[j])
                sa[--bucket[s[j]]] = j;
        }
    }

    vector<int> SA_IS(const vector<int>& s, int K) {
        int n = s.size();
        vector<int> sa(n, -1);
        vector<bool> t(n, false); // true para S‑type, false para L‑type

        // O último símbolo (sentinel) é sempre S‑type
        t[n - 1] = true;
        // Classifica os tipos (S ou L) para cada posição
        for (int i = n - 2; i >= 0; i--) {
            if (s[i] < s[i + 1])
                t[i] = true;
            else if (s[i] > s[i + 1])
                t[i] = false;
            else
                t[i] = t[i + 1];
        }

        // Coleta os índices LMS (posições i tais que t[i] é S e t[i-1] é L)
        vector<int> lms;
        for (int i = 1; i < n; i++) {
            if (t[i] && !t[i - 1])
                lms.push_back(i);
        }

        // Passo 1: Posiciona os LMS no SA usando buckets
        vector<int> bucket;
        getBuckets(s, K, bucket, true);
        for (int pos : lms) {
            sa[--bucket[s[pos]]] = pos;
        }
        sa[0] = n - 1; // Insere o sentinel

        // Induz a ordenação parcial
        induceSort(s, sa, t, K);

        // Passo 2: Extração dos LMS ordenados
        vector<int> sortedLMS;
        for (int i = 0; i < n; i++) {
            if (sa[i] > 0 && t[sa[i]] && !t[sa[i] - 1])
                sortedLMS.push_back(sa[i]);
        }

        // Passo 3: Nomeia as substrings iniciadas em LMS
        int m = lms.size();
        vector<int> lmsNames(n, -1);
        int curName = 0;
        lmsNames[sortedLMS[0]] = curName;
        int prev = sortedLMS[0];
        for (int i = 1; i < (int)sortedLMS.size(); i++) {
            int pos = sortedLMS[i];
            bool diff = false;
            // Compara as duas substrings LMS caractere a caractere.
            for (int d = 0; d < n; d++) {
                if (prev + d == n || pos + d == n) break;
                if (s[prev + d] != s[pos + d] || t[prev + d] != t[pos + d]) {
                    diff = true;
                    break;
                }
                // Se ambos alcançarem um novo LMS (após o primeiro caractere), interrompa a comparação.
                if (d > 0 && ((t[prev + d] && !t[prev + d - 1]) || (t[pos + d] && !t[pos + d - 1])))
                    break;
            }
            if (diff)
                curName++;
            lmsNames[pos] = curName;
            prev = pos;
        }

        // Cria a string resumo dos LMS
        vector<int> summary;
        summary.reserve(m);
        for (int pos : lms) {
            summary.push_back(lmsNames[pos]);
        }

        // Passo 4: Recursão – se os nomes não forem únicos, reconstrua o SA da string resumo
        vector<int> summarySA;
        if (curName + 1 < m) {
            summarySA = SA_IS(summary, curName + 1);
        } else {
            summarySA.resize(m);
            // Se os nomes forem únicos, a ordem é a própria ordem dos nomes
            for (int i = 0; i < m; i++) {
                summarySA[summary[i]] = i;
            }
        }

        // Reordena os LMS conforme a ordem determinada pela string resumo
        vector<int> orderedLMS(m);
        for (int i = 0; i < m; i++) {
            orderedLMS[i] = lms[summarySA[i]];
        }

        // Passo 5: Limpa o SA e posiciona os LMS ordenados para indução final
        fill(sa.begin(), sa.end(), -1);
        getBuckets(s, K, bucket, true);
        for (int i = m - 1; i >= 0; i--) {
            int pos = orderedLMS[i];
            sa[--bucket[s[pos]]] = pos;
        }
        sa[0] = n - 1; // Reinsere o sentinel
        induceSort(s, sa, t, K);

        return sa;
    }
} // namespace SAIS
