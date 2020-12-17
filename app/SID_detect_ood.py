import numpy as np

def load_emp_miu(y):
    path = '..//models//emp_miu_class_' + str(y) + '.npy'
    return np.load(path)

def load_inv_emp_covar():
    path = '..//models//inv_emp_sigma.npy'
    return np.load(path)

def load_mahalanobis_mean(y):
    path = '..//models//mahalanobis_mean_class_' + str(y) + '.npy'
    return np.load(path)

def load_mahalanobis_std(y):
    path = '..//models//mahalanobis_std_class_' + str(y) + '.npy'
    return np.load(path)

def load_mahalanobis_coeff(y):
    path = '..//models//mahalanobis_threshold_class_' + str(y) + '.npy'
    return np.load(path)

def detect_ood(x, y):
       
    emp_miu = load_emp_miu(y)
    inv_emp_sigma = load_inv_emp_covar()
    m_mean = load_mahalanobis_mean(y)
    m_std = load_mahalanobis_std(y)
    coeff = load_mahalanobis_coeff(y)
    
    upper = m_mean + coeff*m_std
    lower = m_mean - coeff*m_std

    try:
        m_dist = np.transpose(x-emp_miu) @ inv_emp_sigma @ (x-emp_miu)
    except:
        m = (x-emp_miu) * inv_emp_sigma * (x-emp_miu)
    
    if lower < m and m < upper:
        return False
    else:
        return True