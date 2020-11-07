#include "../configLoader.h"

/*
 * returns the position in the string the function exited at
 * 
 * complexString is set to TRUE iff the returned string was surrounded by quotes 
*/
int extract_field(string_t str, unsigned int *pos, string_t *dest, int *complexField)
{
    dest->len = 0;
    *complexField = FALSE;
    unsigned int i;
    if (str.len <= *pos)
    {
        return FALSE;
    }

    /*
     * Ignore leading whitespaces
    */
    for (i = *pos; i < str.len; i++)
    {
        if (str.str[i] != CONFIG_IGNORE_CHAR)
        {
            // Done with leading whitespaces
            break;
        }
    }
    *pos = i;

    /*
     * If we've reached an = as its own field
    */
    if (str.str[*pos] == CONFIG_FIELD_DECLARATION_CHAR)
    {
        string_write_char(dest, str.str[*pos]);
        string_null_terminate(dest);
        *pos += 1;
        return TRUE;
    }

    /*
     * If we've reached a # as its own field
    */
    if (str.str[*pos] == CONFIG_COMMENT_CHAR)
    {
        string_write_char(dest, str.str[*pos]);
        string_null_terminate(dest);
        *pos += 1;
        return TRUE;
    }

    /*
     * If we've reached an array declaration square brace
    */
    if (str.str[*pos] == CONFIG_ARRAY_START_CHAR || str.str[*pos] == CONFIG_ARRAY_END_CHAR)
    {
        string_write_char(dest, str.str[*pos]);
        string_null_terminate(dest);
        *pos += 1;
        return TRUE;
    }

    /*
     * If the current field is within quotes
    */
    int inQuotes = FALSE;
    if (str.str[*pos] == CONFIG_STRING_ENCLOSE_CHAR)
    {
        *complexField = TRUE;
        *pos += 1;
        inQuotes = TRUE;
    }

    if (inQuotes)
    {
        /*
         * Read until the string is escaped
        */
        for (i = *pos; i < str.len; i++)
        {
            char c = str.str[i];

            if (c == CONFIG_STRING_ENCLOSE_CHAR)
            {
                // Move to the next character then exit the loop
                i++;
                break;
            }
            if (c == CONFIG_ESCAPE_CHAR && str.str[i + 1] == CONFIG_STRING_ENCLOSE_CHAR)
            {
                // reached a quote intended to be within a string
                string_write_char(dest, str.str[i + 1]);
                i++;
            }
            else
            {
                string_write_char(dest, c);
            }
        }
    }
    else
    {
        /*
         * Read until a whitespace or special char related to
         * the config file is read
        */
        for (i = *pos; i < str.len; i++)
        {
            char c = str.str[i];

            if (c == CONFIG_IGNORE_CHAR)
            {
                // Move to the next character then exit the loop
                i++;
                break;
            }
            else if (c == CONFIG_FIELD_DECLARATION_CHAR ||
                     c == CONFIG_STRING_ENCLOSE_CHAR ||
                     c == CONFIG_COMMENT_CHAR ||
                     c == CONFIG_ARRAY_START_CHAR ||
                     c == CONFIG_ARRAY_END_CHAR)
            {
                break;
            }
            else
            {
                string_write_char(dest, c);
            }
        }
    }

    // Null terminate the field we've extracted
    string_null_terminate(dest);

    // Return the position in the string of the next unread char
    *pos = i;
    return TRUE;
}

array_t read_config_file(char *filePath)
{
    // Storing the processed data
    dynamic_array_t results = new_dynamic_array(sizeof(config_var_t));
    dynamic_array_t fields = new_dynamic_array(sizeof(string_t));
    config_var_t newVar;

    // reading from the file
    FILE *configFile = fopen(filePath, MODE_READ);
    string_t buffer = new_string(DEFAULT_BUFFER_LEN);
    string_t field = new_string(DEFAULT_BUFFER_LEN);

    /*
     * Read every line in the file
    */
    int inArray = FALSE;
    int named = FALSE;
    int currField = CONFIG_FIELD_NAME;
    int complexField = FALSE;
    int currentConfigSaved = FALSE;
    int addedDeclaration = FALSE;

    while (fileio_next_line(configFile, &buffer))
    {

        unsigned int pos = 0;
        while (extract_field(buffer, &pos, &field, &complexField))
        {
            if (!complexField && field.len == 1)
            {
                if (field.str[0] == CONFIG_COMMENT_CHAR)
                {
                    break;
                }
                else if (field.str[0] == CONFIG_FIELD_DECLARATION_CHAR)
                {
                    currField = CONFIG_FIELD_DECLARATION;
                    continue;
                }
                else if (field.str[0] == CONFIG_ARRAY_START_CHAR)
                {
                    inArray = TRUE;
                    continue;
                }
                else if (field.str[0] == CONFIG_ARRAY_END_CHAR)
                {
                    if (!currentConfigSaved)
                    {
                        newVar.data = dynamic_array_to_array(&fields);
                        dynamic_array_append(&results, &newVar);
                        currentConfigSaved = TRUE;
                    }
                    inArray = FALSE;
                    continue;
                }
            }

            if (currField == CONFIG_FIELD_NAME)
            {
                if (!named)
                {
                    currentConfigSaved = FALSE;
                    newVar.varName = string_copy(field);
                    newVar.data.dat = NULL;
                    newVar.data.len = 0;
                    
                    // Reset the array for storing the new data
                    fields.len = 0;
                    named = TRUE;
                    addedDeclaration = FALSE;
                }
            }
            else if (currField == CONFIG_FIELD_DECLARATION)
            {
                if (field.len != 0 && ((!inArray && !addedDeclaration) || inArray))
                {
                    string_t fieldCpy = string_copy(field);

                    // Add the current field to the dynamic array
                    dynamic_array_append(&fields, &fieldCpy);
                }
            }
        }

        if (!inArray)
        {
            if (!currentConfigSaved)
            {
                currentConfigSaved = TRUE;
                newVar.data = dynamic_array_to_array(&fields);
                dynamic_array_append(&results, &newVar);
            }
            currField = CONFIG_FIELD_NAME;
            named = FALSE;
        }
    }
    fclose(configFile);

    if (!currentConfigSaved)
    {
        newVar.data = dynamic_array_to_array(&fields);
        dynamic_array_append(&results, &newVar);
    }

    array_t final = dynamic_array_to_array(&results);

    // Dealing with memory
    string_destroy(&buffer);
    string_destroy(&field);
    
    dynamic_array_destroy(results);
    dynamic_array_destroy(fields);


    return final;
}

/*
 * Free's all dynamically allocated data in a Config
*/
void config_destroy(array_t config)
{
    for (int i = 0; i < config.len; i++)
    {   
        config_var_t var = ((config_var_t*)config.dat)[i];

        string_destroy(&var.varName);
        for (int i = 0; i < var.data.len; i++)
        {
            string_destroy(&((string_t*)var.data.dat)[i]);
        }
        array_destroy(var.data);
    }
    array_destroy(config);
}
